#include <iostream>
#include <fstream>          // for file I/O
#include <cassert>        // for assertion checking
#include <stdlib.h>

using namespace std;


/*
The idea we came up with is to create an array in which will store a fragment of the encrypted text.
The assignment said that the text is written in English so we thought that there has to be at least one "the" in the text.
So we need to create a for loop that will iterate through the values from 0 to 65536 and decode the fragment of text for each value.
We also need to create a function that will check if the word "the" occurs so the letters t h e have to be next to each other in the array.
After finding the value we need to use it on the whole encrypted file and save the output as source.txt

*/

int seed = 0 ;
const int MAX_CHARS = 300;
void initialise_pseudo_random (int r)
{
//  pre-condition:
    assert (r > 0 && r <= 65536) ;
/*  post-condition:
    seed has value r.
*/
    seed = r ;
}

int next_pseudo_random_number ()
{
//  pre-condition:
    assert (seed > 0 && seed <= 65536) ;
/*  post-condition:
    result value > 0 and result value <= 65536 and result value != seed at entry of function
*/
    const int seed75 = seed * 75 ;
    int next = (seed75 & 65535) - (seed75 >> 16) ;
    if (next < 0)
        next += 65537 ;
    seed = next ;
    return next ;
}

char rotate_char (char a, int r)
{
    //Pre-condition:
    assert(r>0&&r<=65536);
    //Post-condition: 0=<a<32
    if (a<32)
        return a;
    if (a>=32)
        return(a-32-(r%(128-32))+(128-32))%(128-32)+32;
}

bool open_input_and_output_file (ifstream& infile, ofstream& outfile)
{
	//  Pre-condition:
	//assert(infile!=outfile);
	//  Post-condition: The file names are stored in a variable. The file information is converted to a string.
	string intxt="secret.txt";
	string outtxt="source.txt";
	//assert(infile!=outfile);
	infile.open(intxt.c_str());
	outfile.open(outtxt.c_str()); //Converts to string
	if(infile&&outfile)
		return true;
	else{
		cout << "Input or output file could not be opened." << endl;
		return false;
	}
}


int initial_encryption_value ()
{// Pre-conditie:
    assert (true) ;
/*  Post-condition:
    result is a value between 0 and 65355 (both inclusive)
*/
    int initial_value = -1 ;
    while (initial_value < 0 || initial_value > 65535)
    {
        cout << "Please enter the initial coding value (greater or equal to 0 and less than 65536)" << endl ;
        cin  >> initial_value ;
    }
    return initial_value ;
}

void use_OTP (ifstream& infile, ofstream& outfile,int initial_value)
{
	//  Pre-condition:
	assert(infile.is_open() && outfile.is_open());
	//  Post-condition:	outputs the decrypted character to the file
	char a;
    char outputchar;
  	initialise_pseudo_random (initial_value);
    while(infile)
    {
    	infile.get(a);
    	outputchar = rotate_char(a, next_pseudo_random_number());
    	outfile << outputchar;
	}
}

void get_fragment (ifstream& infile,char piece [MAX_CHARS])
{
    //  Pre-condition:
	assert(infile.is_open());
	//  Post-condition:	puts the fragment of the text into an array
    char a;
    for (int i=0;i<MAX_CHARS;i++)
    {
         infile.get(a);
         piece[i]=a;
         //cout << piece[i];
    }
}

int decrypt_fragment(char piece [MAX_CHARS])
{
    //  Pre-condition:
	assert(true);
	//  Post-condition:	decrypts the fragment and outputs the correct initial value
    char outputchar;
    char phrase[4]={'T','H','E',' '};
    char test [299];
    for (int i=1;i<65537;i++)
    {
        initialise_pseudo_random (i);
        //cout<<"Initial value: "<<i<<endl;
        for (int n=0;n<MAX_CHARS;n++)
        {
            outputchar = rotate_char(piece[n], next_pseudo_random_number());
            test[n]=outputchar;
            //cout<<test[n];

        }
        //cout << endl;
        for(int j=0;j<MAX_CHARS;j++)
        {
            if(phrase[0]==test[j])
            {
                //cout<<test[j];
                if(phrase[1]==test[j+1])
                {
                    //cout<<test[j+1];
                    if(phrase[2]==test[j+2])
                    {
                        //cout<<test[j+2];
                        if(phrase[3]==test[j+3])
                            return i;
                    }
                }
            }
        }


    }

}
/*
The code doesn't work but was the idea at least close to the solution?
*/

int main()
{
    ifstream input_file ;
    ofstream output_file;
    char fragment [299];
    if (!open_input_and_output_file (input_file,output_file))
    {
        cout << "Program aborted." << endl ;
        return -1 ;
    }
    get_fragment(input_file,fragment);
    const int INITIAL_VALUE = decrypt_fragment(fragment); ;

    use_OTP (input_file,output_file,INITIAL_VALUE);

    input_file.clear () ;
    output_file.clear () ;
    input_file.close () ;
    output_file.close () ;
    if (!input_file || !output_file)
    {
        cout << "Not all files were closed succesfully. The output might be incorrect." << endl ;
        return -1 ;
    }
    return 0 ;
}
