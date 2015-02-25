/**
 * CS 100: Assignment 8: Take as command-line argument, the name of a properly 
 * formatted .csv file with student grades and metadata
 * Example: a.out << testcase.csv
 * 
 * The program runs by "./a.out < sampledata.csv"
 * Compile computegrades with "g++ computegrades.cpp" (Linux)
 * Jonathan Pang
 * jpang005
 * Assn 8
 */
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std; 
//User must input as command line arguments the name of the .csv file
//This file must be properly formatted

//Input format of csv file:
// The top three rows will contain metadata:
//- The first contains the respective weights for each category: assign-
//ments, labs, quizzes, and final. These should add up to 100.
//- The second contains the cutoffs, respectively, for A+, A, A-, B+, B,
//B-, C+,C, C-, D+,D,D-, and F.
//- The third contains the labels telling which kind of data is found in the
//corresponding column: name, junk, asst, lab, quiz, final.
//
//- Each of the rest of the rows contains the name and scores for a given
//student plus perhaps some other information to be ignored. Treat each
//score as a floating-point number.
class Normalized_Data
{
	public:
	string s_name;
	vector<int> quiz;
	vector<int> asst;
	vector<int> lab;
	vector<int> final;
};

//helper functions
//
//

//operator << for vector <int>
ostream& operator << (ostream& op, vector <string> out)
{
	for(vector<string>::iterator it = out.begin(); it != out.end(); ++it)
	{
			op << *it << " ";
	}
}

//overloaded operator for  vector<int>
ostream& operator << (ostream& op, vector <int> out)
{
	for(vector<int>::iterator it = out.begin(); it != out.end(); ++it)
	{
			op << *it << ",";
	}
}

vector<string> split( const string s, const string pat )
// Returns the vector of fragments (substrings) obtained by splitting
// s at occurrences of pat. Use C++'s string::find. If s begins with
// an occurrence of pat, it has an empty-string fragment. Similarly
// for trailing occurrences of pat. After each occurrence of pat,
// scanning resumes at the following character, if any.
{
	//cout << "splitting " << s << " by " << pat <<endl;			//checking to see if correct inputs
	vector<string> separated_strings;
	if(s.empty())
	{
		return separated_strings;
	}
	int pat_length = pat.size();
	int substr_start = 0;
	string s_copy = s;
	bool end = false;
	if((int)s_copy.find(pat) == -1)
	{
		separated_strings.push_back(s_copy);
		return separated_strings;
	}
	while(end == false && s_copy.find(pat)>=0)
	{
		if(s_copy.find(pat)==0)
		{
			separated_strings.push_back("");
		}
		else
		{
			separated_strings.push_back(s_copy.substr(substr_start,s_copy.find(pat)));
			//cout << s_copy.substr(substr_start,s_copy.find(pat)) << " ";			//checking to see if string split properly
		}
		
		if(s_copy.find(pat) == -1)		//check to see if string completely parsed through
		{
			end = true;
		}
		else 		//recreates string with first part removed
		{
			s_copy = s_copy.substr(s_copy.find(pat) + pat_length);
		}
	}
	return separated_strings;
}



string join(const vector<string> v, const string pat, int start=0, int n=INT_MAX)
// Joins (concatenates) members of v separated by occurrences of pat into
// a single string and returns it. The value of start is the index of the
// first member to be joined, and n bounds the number of members to be
// joined.
{
	string concatenate = "";
	if(start >= (int)v.size() || start <0)
	{
		string fail = "start index is out of bounds. Error.\n";
		return fail;
	}
	for(int i = start; i < (int)v.size(); i++)
	{
		if(i+1 == (int)v.size())
		{
			concatenate = concatenate + v[i];
		}
		else
		{
			concatenate = concatenate + v[i] + pat;
		}
	}
	
	return concatenate;
}

//main function 
int main(int argc, char* argv[])
{
	if(argc <= 1)
	{
		cout << "No command-line arguments entered."<<endl;
		return 0;
	} 
	else if(argc == 2)
	{
		//Variables
		string weights = "", cutoffs = "";
		string column_labels = "";
		string student_line = "";
		int line_number = 0;
		char* file_name = argv[1];
		string s = file_name;
		vector<int> weights_i;
		vector<int> cutoffs_i;
		vector <int> high_scores;
		vector<string> labels;
		vector<string> student_data;
		vector< vector<string> > students;
		vector<string> highest_scores;
		vector< vector<string> > normal;
		ifstream myfile;
		
		//Code
		myfile.open(file_name);
		if(myfile.good())
		{
			//cout << "exists"<<endl;
			myfile.close();
			string command = file_name;
			string newfile;
			command = command.substr(0,command.length()-4);		// removes .csv suffix
			command = command + ".txt";							// command now says (prog_name).txt
			newfile = command;									// copies command into newfile
			command = "cat " + s + " > " + command;			// command system to convert .csv file to a .txt file
			system(command.c_str());
			myfile.open(newfile.c_str());

			//begin collection
			while(myfile.good())
			{
				if(line_number == 0)
				{
					//collecting weights
					//cout <<"weights"<<endl;
					myfile >> weights;
					weights = weights.substr(0,weights.length()-1);
					weights_i.push_back(atoi(weights.c_str()));
					//labs weight
					myfile >> weights;
					weights = weights.substr(0,weights.length()-1);
					weights_i.push_back(atoi(weights.c_str()));
					//quizzes weight
					myfile >> weights;
					weights = weights.substr(0,weights.length()-1);
					weights_i.push_back(atoi(weights.c_str()));
					//final weight
					myfile >> weights;
					weights_i.push_back(atoi(weights.c_str()));
					//getline(myfile,weights);
					//cout << weights << endl;

					//collecting cutoffs line
					//A+ cutoff
					myfile >> cutoffs;
					cutoffs = cutoffs.substr(0,cutoffs.length()-1);
					cutoffs_i.push_back(atoi(cutoffs.c_str()));
					//A cutoff
					myfile >> cutoffs;
					cutoffs = cutoffs.substr(0,cutoffs.length()-1);
					cutoffs_i.push_back(atoi(cutoffs.c_str()));
					//A- cutoff
					myfile >> cutoffs;
					cutoffs = cutoffs.substr(0,cutoffs.length()-1);
					cutoffs_i.push_back(atoi(cutoffs.c_str()));
					//B+ cutoff
					myfile >> cutoffs;
					cutoffs = cutoffs.substr(0,cutoffs.length()-1);
					cutoffs_i.push_back(atoi(cutoffs.c_str()));
					//B cutoff
					myfile >> cutoffs;
					cutoffs = cutoffs.substr(0,cutoffs.length()-1);
					cutoffs_i.push_back(atoi(cutoffs.c_str()));
					//B- cutoff
					myfile >> cutoffs;
					cutoffs = cutoffs.substr(0,cutoffs.length()-1);
					cutoffs_i.push_back(atoi(cutoffs.c_str()));
					//C+ cutoff
					myfile >> cutoffs;
					cutoffs = cutoffs.substr(0,cutoffs.length()-1);
					cutoffs_i.push_back(atoi(cutoffs.c_str()));
					//C cutoff
					myfile >> cutoffs;
					cutoffs = cutoffs.substr(0,cutoffs.length()-1);
					cutoffs_i.push_back(atoi(cutoffs.c_str()));
					//C- cutoff
					myfile >> cutoffs;
					cutoffs = cutoffs.substr(0,cutoffs.length()-1);
					cutoffs_i.push_back(atoi(cutoffs.c_str()));
					//D+ cutoff
					myfile >> cutoffs;
					cutoffs = cutoffs.substr(0,cutoffs.length()-1);
					cutoffs_i.push_back(atoi(cutoffs.c_str()));
					//D cutoff
					myfile >> cutoffs;
					cutoffs = cutoffs.substr(0,cutoffs.length()-1);
					cutoffs_i.push_back(atoi(cutoffs.c_str()));
					//D- cutoff
					myfile >> cutoffs;
					cutoffs = cutoffs.substr(0,cutoffs.length()-1);
					cutoffs_i.push_back(atoi(cutoffs.c_str()));
					//F cutoff
					myfile >> cutoffs;
					cutoffs_i.push_back(atoi(cutoffs.c_str()));
					//cout << cutoffs << endl;  		//should be 0
					

					//collecting column_labels data
					myfile >> column_labels;
					//cout << cutoffs;
					labels = split(column_labels, ",");
					//cutoffs = join(labels, " ");
					//cout << labels;
				}
					
				else
				{
					myfile >> student_line;						//getline(myfile,student_line);
					student_data = split(student_line, ",");
					//highest_scores = split(student_line, ",");
					students.push_back(student_data);
//					//cout << student_data;											//student_data output
					//cout << endl;
					//cout << student_line;
					//cout << endl;
				}
					line_number++;
				
			}
			myfile.close();
			students.pop_back();	//deletes duplicate data at end of students
			highest_scores = student_data;
				//students now contains a vector of all student_data vectors 
				//labels contains column labels for the student_data vectors
				//cutoffs_i contains the grade cutoffs for each grade
				//weights_i contains the weights for (asst, labs, quizzes, final) in that exact order

				/*			checking outputs
			cout << cutoffs_i;
			cout <<endl;
			cout << weights_i;
			cout << endl;
			cout << labels;
			cout << endl;
			cout << highest_scores;
			cout << endl;
			cout << "recalculating high scores" << endl;
			*/
			//now we want to find the highest values for each asst, lab, quiz, final
			highest_scores[0] = "Top Grades";
			for(unsigned s_i = 0; s_i < students.size(); s_i++)
			{
				for(unsigned i = 0; i< labels.size(); i++)
				{
					
					if(labels[i] == "\"asst\"" || labels[i] == "\"lab\"" || labels[i] == "\"quiz\"" || labels[i] == "\"final\"")
					{
						//cout << "comparing" << endl;
						if(highest_scores.empty())
						{
							cout<< "empty" <<endl;
							exit(-1);
						}
						if(highest_scores[i] == "")
						{
							//cout << "replacing " << highest_scores[i] << " with 0" << endl;
							highest_scores[i] = "0";
							i--;
						}
						else if(atof(highest_scores[i].c_str()) < atof((students[s_i])[i].c_str()))
						{
							//cout << "Replacing " << highest_scores[i] << " with " << (students[s_i])[i] << endl;
							highest_scores[i] = (students[s_i])[i];
							i--;
						}
					}
				}
			}
			//cout << highest_scores;
			//cout << endl;
			//highest_scores now contains the maximum possible point value
			// we now want to normalize all the other students
			normal = students;
			for(unsigned n_i = 0; n_i < normal.size(); n_i++)
			{
				for(unsigned i = 0; i< labels.size(); i++)
				{
					
					if(labels[i] == "\"asst\"" || labels[i] == "\"lab\"" || labels[i] == "\"quiz\"" || labels[i] == "\"final\"")
					{
						stringstream dbl_stream;
						dbl_stream << (atof(((normal[n_i])[i]).c_str())/atof(highest_scores[i].c_str()));
							(normal[n_i])[i] = dbl_stream.str();
					}
				}
			}
			//normal now contains the normalized scores
			//now we want to throw out the two lowest quiz scores
			vector<string> header = labels;
			for(unsigned n_i = 0; n_i < normal.size(); n_i++)
			{
				unsigned lowest_pos_1 = 0;
				unsigned lowest_pos_2 = 0;
				float lowest_1 = 100;
				float lowest_2 = 100;
				float lab_avg = 0;
				unsigned lab_count = 0;
				float quiz_avg = 0;
				unsigned quiz_count = 0;
				float asst_avg = 0;
				unsigned asst_count = 0;
				float final_avg = 0;
				for(unsigned i = 0; i< labels.size(); i++)
				{
					if(labels[i] == "\"quiz\"")
					{
						if(atof(((normal[n_i])[i]).c_str()) < lowest_2)
						{
							lowest_2 = atof(((normal[n_i])[i]).c_str());
							lowest_pos_2 = i;
						}
						if(lowest_2 < lowest_1)
						{
							float low_holder = lowest_1;
							unsigned low_pos_holder = lowest_pos_1;
							lowest_1 = lowest_2;
							lowest_pos_1 = lowest_pos_2;
							lowest_2 = low_holder;
							lowest_pos_2 = low_pos_holder;
						} 
					}
				}
				//now we know the positions of the two quiz scores we want to ignore.
				//we now want to calculate the respective averages on labs, asst, quiz and final
				//use those to calculate the weighted averages for each category and then find
				//the overall score and grade and put them onto the report sheet
				
				for(unsigned i = 0; i< labels.size(); i++)
				{		
					if(labels[i] == "\"asst\"")
					{
						asst_avg+=atof(((normal[n_i])[i]).c_str());
						asst_count++;
					}
					if(labels[i] == "\"lab\"" )
					{
						lab_avg+=atof(((normal[n_i])[i]).c_str());
						lab_count++;
					}
					if(labels[i] == "\"quiz\"")
					{
						if(i != lowest_pos_1)
						{
							if(i != lowest_pos_2)
							{
								quiz_avg += atof(((normal[n_i])[i]).c_str());
								quiz_count++;
							}
						}
					}
					if(labels[i] == "\"final\"")
					{
						final_avg = atof(((normal[n_i])[i]).c_str());
					}
				}
				
				asst_avg = asst_avg/asst_count;
				lab_avg = lab_avg/lab_count;
				quiz_avg = quiz_avg/quiz_count;
				//averages calculated
				//inserting average quiz into report
				stringstream dbl_stream2;
				dbl_stream2 << int(quiz_avg*weights_i[2]+.5);
				string data2 = dbl_stream2.str();
				(students[n_i]).insert(students[n_i].begin()+1, data2);
				//inserting average asst into report
				stringstream dbl_stream3;
				dbl_stream3 << int(asst_avg*weights_i[0]+.5);
				string data3 = dbl_stream3.str();
				(students[n_i]).insert(students[n_i].begin()+1, data3);
				//inserting average lab into report
				stringstream dbl_stream4;
				dbl_stream4 << int(lab_avg*weights_i[1]+.5);
				string data4 = dbl_stream4.str();
				(students[n_i]).insert(students[n_i].begin()+1, data4);
				//calculating overall score
				float overall = asst_avg*weights_i[0] + lab_avg*weights_i[1] + quiz_avg*weights_i[2] + final_avg*weights_i[3];
				string grade;
				
				if(overall > (float) cutoffs_i[0])
				{
					grade = "A+";
				}
				else if(overall > (float) cutoffs_i[1])
				{
					grade = "A";
				}
				else if(overall > (float) cutoffs_i[2])
				{
					grade = "A-";
				}
				else if(overall > (float) cutoffs_i[3])
				{
					grade = "B+";
				}
				else if(overall > (float) cutoffs_i[4])
				{
					grade = "B";
				}else if(overall > (float) cutoffs_i[5])
				{
					grade = "B-";
				}else if(overall > (float) cutoffs_i[6])
				{
					grade = "C+";
				}else if(overall > (float) cutoffs_i[7])
				{
					grade = "C";
				}
				else if(overall > (float) cutoffs_i[8])
				{
					grade = "C-";
				}
				else if(overall > (float) cutoffs_i[9])
				{
					grade = "D+";
				}
				else if(overall > (float) cutoffs_i[10])
				{
					grade = "D";
				}
				else if(overall > (float) cutoffs_i[11])
				{
					grade = "D-";
				}
				else 
				{
					grade = "F";
				}
				//insert grade into report
				(students[n_i]).insert(students[n_i].begin(), grade);
				//insert overall score into report
				stringstream overall_int;
				overall_int << int(overall+.5);
				string overall_data = overall_int.str();
				(students[n_i]).insert(students[n_i].begin(), overall_data);

				
			}
			string quiz_title = "quiz_avg";
			header.insert(header.begin()+1,quiz_title);
			header.insert(header.begin()+1,"asst_avg");
			header.insert(header.begin()+1,"lab_avg");
			header.insert(header.begin(),"grade");
			header.insert(header.begin()+1,"overall_score");
			//students now contains vector of all students
			//header now contains the properly named columns
			//output the final report
			freopen("report2.csv","w", stdout);
			cout << header;
			cout << endl;
			for(unsigned s_i2 = 0; s_i2 < students.size(); s_i2++)
			{
				cout << students[s_i2];
				cout << endl;
			}
			fclose(stdout);
			string command_sort = "sort -g report2.csv > report.csv";
			system(command_sort.c_str());
			string command_unlink1 = s.substr(0,s.length()-4) + ".txt";
			string command_unlink2 = "report2.csv";
			unlink(command_unlink1.c_str());
			unlink(command_unlink2.c_str());
		}		
		else
		{
			cout << "File does not exist" << endl;
			return 0;
		}
	}
	else
	{
		cout << "Too many arguments entered."<<endl;
		return -1;
	}
	return 0;
}
