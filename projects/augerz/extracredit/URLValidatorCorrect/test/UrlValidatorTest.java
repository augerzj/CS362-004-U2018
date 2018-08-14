import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import java.util.Random;
import org.junit.Test;



public class UrlValidatorTest { 
	
   final int NUM_TEST_RUNS = 100000;
	
   @Test
   public void isValidRandomTest()
   {
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   for (int numTests = 0; numTests < NUM_TEST_RUNS; numTests++) {
		   String urlToTest = "";
		   boolean isValidUrl = true;
		   
		   // pick components of string
		   int[] rowIndices = new int[]{-1, -1, -1, -1, -1};
		   int[] colIndices = new int[]{-1, -1, -1, -1, -1};
		   getRandRowsAndCols(rowIndices, colIndices);
		   
		   // build string
		   for (int i = 0; i < 5; i++) {
			   if (colIndices[i] != 4) {
				   urlToTest += urlComponents[rowIndices[i]][colIndices[i]];
			   }
		   }
		   
		   // test validity of string
		   // filter out blank components
		   int counter = 0;
		   int[] nonBlankRowIndices = new int[5];
		   int[] nonBlankColIndices = new int[5];
		   for (int i = 0; i < 5; i++) {
			   if (colIndices[i] != 4) {
				   nonBlankRowIndices[counter] = rowIndices[i];
				   nonBlankColIndices[counter] = colIndices[i];
				   counter++;
			   }
		   }
		   
		   // check that url has scheme and authority in correct places
		   if (nonBlankRowIndices[0] != 0) {
			   isValidUrl = false;
		   }
		   if (nonBlankRowIndices[1] != 1) {
			   isValidUrl = false;
		   }
		   
		   // check that url components are valid
		   for (int i = 0; i < counter; i++) {
			   if (i < 3 && !urlComponentValidity[nonBlankRowIndices[i]][nonBlankColIndices[i]]) {
				   isValidUrl = false;
				   break;
			   }
			   
			   if (i >= 3) {
				   if (nonBlankRowIndices[i] == 4 && (nonBlankColIndices[i] == 2 || nonBlankColIndices[i] == 3)) {
					   isValidUrl = false;
					   break;
				   }
				   if (i == 3 && nonBlankRowIndices[2] == 2 && 
					nonBlankRowIndices[i] == 3 && (nonBlankColIndices[i] == 2 || nonBlankColIndices[i] == 3)) {
					   isValidUrl = false;
					   break;
				   }
			   }
		   }
		   
		   // compare isValid results with expected results
		   if (isValidUrl) {
			   assertTrue(urlToTest + " should be valid", urlVal.isValid(urlToTest));
		   }
		   else {
			   assertFalse(numTests + urlToTest + " should be invalid", urlVal.isValid(urlToTest));
		   }
	   }
   }
   
   public void getRandRowsAndCols(int[] rowIndices, int[] colIndices) {
	   Random rand = new Random();
	   
	   // get the 5 different component row indices (need to be unique)
	   int counter = 0;
	   while (counter < 5) {
		   boolean foundUnusedIndex = false;
		   while (!foundUnusedIndex) {
			   boolean isDuplicate = false;
			   int randIndex = rand.nextInt(5);
			   for (int i = 0; i < counter; i++) {
				   if (rowIndices[i] == randIndex) {
					   isDuplicate = true;
				   }
			   }
			   if (!isDuplicate) {
				   rowIndices[counter] = randIndex;
				   counter++;
				   foundUnusedIndex = true;   
			   }
		   }
	   }
	   
	   // get the 5 different component col indices
	   for (int i = 0; i < 5; i++) {
		   colIndices[i] = rand.nextInt(5);
	   }
   }
   
   public static void main() {
      UrlValidatorTest randTester = new UrlValidatorTest();
      randTester.isValidRandomTest();
   }
   
   String[][] urlComponents = new String[][]{
	   {"http://", "https://", "g2s:://", "293://"},
	   {"www.google.com", "www.gmail.com", "asdf.2342", "2342.23432.234"},
	   {":2304", ":25923", ":-12351", ":::::"},
	   {"/recipes/turkey/sandwich", "/emails", "//flyers", "/asdf//asd/"},
	   {"?type=dog", "?element=earth&name=toph", "? run=true", "?fly=false & run=true"}
   };
   
   boolean[][] urlComponentValidity = new boolean[][]{
	   {true, true, false, false},
	   {true, true, false, false},
	   {true, true, false, false},
	   {true, true, false, false},
	   {true, true, false, false}
   };
}