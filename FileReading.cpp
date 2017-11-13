//////////////////////////////////////////////////////////////
//
// Your #includes here; make sure you are allowed them ...
//
#include <fstream>
using namespace std;
int maxNumErrors = 10;
//////////////////////////////////////////////////////////////
//
// #includes and function declarations; do not remove
//

#ifndef MARMOSET_TESTING
#include <iostream>
using namespace std;
#endif

//////////////////////////////////////////////////////////////
//
// Function declarations; do not remove
// Replace "..." in bisectionHelper() with any parameter(s)
// you need to ensure termination when precision and
// minIntervalSize are too tightly specified.
//
enum type {WS1, StudentID, WS2, COMMA, GRADE, WS3, WS4,END};
int histogram(const char filename[], int histogram[10], 
              const int minAcceptableID, const int maxAcceptableID,
              int*& rejects);

//////////////////////////////////////////////////////////////
//
// Your code here ...
//  
// Return -1 if you cannot compute the histogram for any
// reason;
// Return 0 if the histogram is correct and there are no
// rejections;
// Return the number of rejects if there are rejected
// records from the file
//
int parse(char* line, const int minAcceptableID, const int maxAcceptableID, int maxLineLength, int& sn, int& g){
	type state=WS1;
	int i=0;
	sn=0;
	g=0;
	while(line[i]!=0){
	switch(state){
		case WS1:
			if(line[i]==' '||line[i]=='\t'){
				state=WS1;
				break;
			}
			else if(line[i]>='0'&&line[i]<='9'){
				state=StudentID;
			}
			else if (line[i]==','){
					state = COMMA;
					break;
				}
			else{
				return -2;
			}
			
		case StudentID:
			if(line[i]>='0'&&line[i]<='9'){
				sn=sn*10+(line[i]-48);
				break;
			}
			else if(line[i]==' '||line[i]=='\t'){
				state=WS1;
				break;
			}
			else if(line[i]==','){
				state=COMMA;
				break;
			}
			else{
				return -2;
			}
			
		case COMMA:
			if(line[i]>='0'&&line[i]<='9'){
				state=GRADE;
			}
			else if(line[i]==' '||line[i]=='\t'){
				state=WS2;
				break;
			}
			else{
				return -2;
			}
				
		case WS2:
			if(line[i]==' '||line[i]=='\t'){
				state=WS2;
				break;
			}
			else if(line[i]>='0'&&line[i]<='9')
				state=GRADE;

			else{
				return -2;
			}
			
		case GRADE:
			if(line[i]>='0'&&line[i]<='9'){
				g=g*10+(line[i]-48);
				break;
			}
			else{
				break;
			}
	}
		i++;
	}
	if(sn<=maxAcceptableID&&sn>=minAcceptableID&&g>=0&&g<=100)
		return 0;
	else
		return -1;
}


int histogram(const char filename[], int histogram[10], 
              const int minAcceptableID, const int maxAcceptableID,
              int*& rejects) {
	if(minAcceptableID>maxAcceptableID)
		return -1;
	if (histogram == NULL || histogram == 0)
		return -1;
	int a=0; 
	rejects=new int [maxNumErrors];
	int numr=0;
	const int maxLineLength = 100;
	char line[maxLineLength];
	for(int i=0; i<10; i++)
		histogram[i]=0;
	
	ifstream infile;
	infile.open(filename);
	if(!infile.is_open())
		return -1;
	bool done=false;
	int fln=0;
	while(!done){
		int sn=0;
		int g=0;
		++fln;
		if(!infile.getline(line,maxLineLength)){
			if(infile.eof()){
				done=true;
			}
			else{
				return -1;
			}	
		}
	if (!done){
		int num= parse(line,minAcceptableID,maxAcceptableID,maxLineLength,sn,g);
			if(num==-1){
				numr++;
				if(numr>maxNumErrors)
					return -1;
				rejects[a]= fln;
				a++;
			}
			else if(num==-2)
				return -1;
			else{
				if(g==100)
					histogram[9]++;
				else
					histogram[g/10]++;
			}
		}
	}
	return numr;
}

//////////////////////////////////////////////////////////////
//
// Test Driver
//
// Do not remove the #ifndef and its associated #endif
// This is required so that when you submit your code,
// this test driver will be ignored by Marmoset
//

#ifndef MARMOSET_TESTING

#define isNaN(X) (X != X)  // NaN is the only float that is not equal to itself

int main(const int argc, const char* const argv[]) {
  
  // Some test driver code here ....
  if (argc < 2) {

  }
  const int numBuckets = 10;
  const int bucketRange = 100/numBuckets;
  int  buckets[numBuckets];
  int* rejectedRecords;
  int retCode = histogram(argv[1], buckets, 22200000, 22299999, rejectedRecords);
  if (retCode < 0) {
      cout << "Unable to compute histogram." << endl;
  }
  else {
    for (int i = 0; i < numBuckets; ++i)
      if (i != numBuckets-1)
	cout << "Number [" << i*bucketRange << "," << (((i+1)*bucketRange)-1) << "]: " << buckets[i] << endl;
      else
	cout << "Number [" << i*bucketRange << "," << (((i+1)*bucketRange)) << "]: " << buckets[i] << endl;
    if (retCode > 0) {
      cout << "Number of rejected records: " << retCode << endl;
      cout << "Rejected records are:";
      for (int i = 0; i < retCode; ++i)
	cout << " " << rejectedRecords[i];
      cout << endl;
      delete[] rejectedRecords;
    }
  }

  return 0;
}

#endif
