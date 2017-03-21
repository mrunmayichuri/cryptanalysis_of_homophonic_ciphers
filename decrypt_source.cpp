/*
----------------------------------------------------------
Cryptanalysis of Homophonic Ciphers
----------------------------------------------------------
*/

#include<iostream>
#include<time.h>
#include<map>
#include<vector>
#include<sstream>
#include<fstream>


using namespace std;

#define MAX_KEY_VAL 103

class CryptAnalysis
{
    int test_no,ctr;
  	string ciphertext;
  	vector<string> dict_words;
  	vector< vector<int> > cipher_num;
  	vector<int> words_length_list, used_keys, char_count;
  	string plain_text, word, s;
  	map<char, int> key_frequency;
  	float total_time;
  	clock_t begin_time;
    vector<string> wordlist;
  	map<int, vector<string> > dictionary_length_map;

	struct Encipher
	{
  		bool abort;
  		map<char, int> char_freq;
  		vector<string> dict_words;
  		vector<vector<int> > cipher_num;
  		char replaced[MAX_KEY_VAL];
  		vector<int> used_keys;
	};

	Encipher en_scheme;

  public:

	CryptAnalysis(string ciphertext, int test_no)
	{
		ctr = 0;
    	// gettting ciphertext for processing
		this->ciphertext = ciphertext;
    	// test case 1
		if (test_no == 1)
		{
      	// read from file test case 1
			ifstream input("plaintext_dictionary.txt");
      	// This needs a space delimiter as the dictionary is separated by spaces and it is made up of paragraphs
      while(getline(input, word, ' '))
			{
        		// populate the word list
				this->wordlist.push_back(word);
			}
			input.close();
		}
    	// test case 2
		else
		{
      		// read from file test case 2
			ifstream input("english_words.txt");
			while(getline(input, word))
			{
        		// populate the word list
				this->wordlist.push_back(word);
			}
			input.close();
		}

    	// processing the ciphertext from the file
		for(int i=0;i<this->ciphertext.size();i++)
		{
      		// if space is found create new word in word list
			if(this->ciphertext[i] == ' ')
			{
        		// copy the word on to the list
				this->dict_words.push_back(s);
        		// copy the length of word
				this->words_length_list.push_back(ctr);
        		// copy the number of characters in the word in cipher_num
        		this->cipher_num.push_back(char_count);
        		// clear character count for next word
				char_count.clear();
        		// clear the word
				s.clear();
        		//reset the word length count
				ctr=0;
			}
      		// if encounter , then proceed to next character
			else if(this->ciphertext[i] == ',')
				continue;
			else
			{
        		// get encrypted characters as numbers
				int digit, num=0;
        		// check if number between 0 and 9
				while(this->ciphertext[i]<='9' && this->ciphertext[i]>='0')
				{
          			// type casting
					digit = this->ciphertext[i] - '0';
					num = num*10 + digit;
					i++;
				}
        		// replace characters with -
				s.push_back('-');
				ctr++;
        		// character count saved
				char_count.push_back(num);
				i--;
			}
		}

		// populates the length of the words populated from the word dictionary
		for(int i=0; i<this->wordlist.size(); i++)
		{
      		// populate the dictionary with words and word length
			this->dictionary_length_map[this->wordlist[i].size()].push_back(this->wordlist[i]);
		}

		// last word after encountering the last comma
		dict_words.push_back(s);
    	// word length to corresponding last word
		words_length_list.push_back(ctr);
    	// save character count of last word`
		cipher_num.push_back(char_count);
		s.clear();
		ctr=0;
    	// initialise frequency map
		this->init_key_frequency_table();
	}

	~CryptAnalysis()
	{

	}

  //initialization of empty key frequency table
  void init_key_frequency_table()
	{
    	// key frequency
		map<char, int> key_frequency;

		key_frequency['a'] = 8;
		key_frequency['b'] = 1;
		key_frequency['c'] = 3;
		key_frequency['d'] = 4;
		key_frequency['e'] = 13;
		key_frequency['f'] = 2;
		key_frequency['g'] = 2;
		key_frequency['h'] = 6;
		key_frequency['i'] = 7;
		key_frequency['j'] = 1;
		key_frequency['k'] = 1;
		key_frequency['l'] = 4;
		key_frequency['m'] = 2;
		key_frequency['n'] = 7;
		key_frequency['o'] = 8;
		key_frequency['p'] = 2;
		key_frequency['q'] = 1;
		key_frequency['r'] = 6;
		key_frequency['s'] = 6;
		key_frequency['t'] = 9;
		key_frequency['u'] = 3;
		key_frequency['v'] = 1;
		key_frequency['w'] = 2;
		key_frequency['x'] = 1;
		key_frequency['y'] = 2;
		key_frequency['z'] = 1;

    	// possible max random number generated by encryption scheme
		for(int i=0;i<MAX_KEY_VAL;i++)
		{
      		// replace all characters by -
			this->en_scheme.replaced[i] = '-';
		}
    	// copy key frequency to encryption scheme
		this->en_scheme.char_freq = key_frequency;
    	// dictionary words from test case
		this->en_scheme.dict_words = this->dict_words;
    	// word length of the dictionary word
		this->en_scheme.cipher_num = this->cipher_num;
    	// default abort value
		this->en_scheme.abort = false;
	}

  
   //length of the word with the least frequency of occurence
   
	int get_min_freq_word_length(vector<int> cipher_length_list)
 	{
   		int num_words = this->dictionary_length_map[cipher_length_list[0]].size();
   		int length = cipher_length_list[0];
   		for(int i=1;i<cipher_length_list.size();i++)
   		{
     		if(num_words>this->dictionary_length_map[cipher_length_list[i]].size())
     		{
       			num_words = this->dictionary_length_map[cipher_length_list[i]].size();
       			length = cipher_length_list[i];
     		}
   		}
   		return length;
 	}

  	// check pattern comparing dictionary word to ciphertext word at character level
 	bool check_pattern(string dic_word, string word)
 	{
   		for(int i=0;i<word.size();i++)
   		{
        	// if ciphertext is not replaced
     		if(word[i] == '-')
       		continue;
     		else
     		{
            	// if no character match between ciphertext word and dictionary word
       			if(word[i] != dic_word[i])
         		return false;
     		}
   		}
   		return true;
 	}

  	// return matched pattern at word length level
 	vector<string> match_pattern(string word)
 	{
      	// matching word of length from dictionary to ciphertext word length
   		vector<string> matches = this->dictionary_length_map[word.size()];;
   		int i=0;
      	// process until the length of ciphertext word
   		while(i<matches.size())
   		{
     		if(!check_pattern(matches[i], word))
     		{
            	// delete the character at the position
       			matches.erase(matches.begin() + i);
     		}
     		else
       		i++;
   		}
   		return matches;
 	}

	void decryption()
	{
    	// begin the clock
		this->begin_time = clock();
    	// get minimum common length of ciphertext word and dictionary word
		int length_least_freq_word = this->get_min_freq_word_length(this->words_length_list);
    	// get words of dictionary matching length
		vector<string> dictionary_words = this->dictionary_length_map[length_least_freq_word];
    	// process words of dictionary words
		for(int i=0;i<dictionary_words.size();i++)
		{
      		// get first dictionary word
			string first_word = dictionary_words[i];
      // instantiate encryption scheme
      Encipher temp_map = this->en_scheme;
      // encryption scheme unitl the size of dictionary word length
      for(int i=0;i<temp_map.dict_words.size();i++)
			{
        		// process if dictinoary word length to length of ciphertext word
				if(temp_map.dict_words[i].size() == first_word.size())
				{

					temp_map = update_cipher_keys(temp_map, first_word, i);
					temp_map.dict_words[i] = first_word;
					vector<int> keys = temp_map.cipher_num[i];

					for(int j=0;j<temp_map.dict_words[i].size();j++)
					{
						temp_map = replace_keys(keys[j], temp_map.dict_words[i][j], temp_map);
					}
					break;
				}
			}
			if(temp_map.abort)
				continue;

			if(this->check_en_scheme_completion(temp_map.dict_words))
			{
				this->print_decrypted_output(temp_map.dict_words, temp_map.cipher_num);
				continue;
			}
			// Now call the recursive function to repeatedly check for matches.
			process_cipher_text(temp_map);
		}
			cout<<endl<<"Decryption time : "<<this->total_time<<" seconds"<<endl;
	}


	Encipher update_cipher_keys(Encipher temp_map, string word, int index)
	{
		string next_word = temp_map.dict_words[index];
		vector<int> next_keys = temp_map.cipher_num[index];

		// key already used
		for(int i=0;i<next_keys.size();i++)
		{
			if(next_word[i] != '-')
			{
				// key already replaced, do nothing
				continue;
			}
			else if(temp_map.replaced[next_keys[i]] != '-')
			{
				// key already used earlier in the same word
				bool abort = true;
				for(int j=0;j<i;j++)
				{
					if(next_keys[i] == next_keys[j])
						abort = false;
				}
				if(abort)
				{
					// Key already used, abort
					temp_map.abort = true;
					return temp_map;
				}
			}
			else if(temp_map.char_freq[word[i]] == 0)
			{
				// Character frequency over, abort
				temp_map.abort = true;
				return temp_map;
			}
			else
			{
				temp_map.replaced[next_keys[i]] = word[i];
				temp_map.char_freq[word[i]]--;
			}
		}

		return temp_map;
	}

  	// replace the matching word
	Encipher replace_keys(int key, char c, Encipher temp_map)
	{
		for(int i=0;i<temp_map.cipher_num.size();i++)
		{
			vector<int> keys = temp_map.cipher_num[i];
      		// get word from the dictionary
			string s = temp_map.dict_words[i];
			for(int j=0;j<keys.size();j++)
			{
        		// if key matched to the character
				if(keys[j] == key)
				{
          			// replace characters by key
					s[j] = c;
				}
			}
      		// replace word in encryption scheme
			temp_map.dict_words[i] = s;
		}

		return temp_map;
	}

	void process_cipher_text(Encipher temp_map)
	{
		int index = 0;
		int max = -1;
		for (int i = 0; i<temp_map.dict_words.size(); i++)
		{

			string s = temp_map.dict_words[i];
			int filled = 0;

			for (int j = 0; j<s.size(); j++)
			{
        		// check if filled
				if (s[j] != '-')
					filled++;
			}
      		// if character is not  filled, update
			if (filled != s.size() && filled>max)
			{
				max = filled;
				index = i;
			}
		}
		// recursively check for all dict_words of same length from the dictionary.
		vector<string> matching_words = this->match_pattern(temp_map.dict_words[index]);

    	// no matching left, exit
		if(matching_words.size() == 0)
		{
			return;
		}

		for(int i=0;i<matching_words.size();i++)
		{
			Encipher cipher_scheme = temp_map;
      // update encryption scheme
      cipher_scheme = update_cipher_keys(cipher_scheme, matching_words[i], index);
      		// if not abort
			if(cipher_scheme.abort)
				continue;
      		// get matching words to  length
			cipher_scheme.dict_words[index] = matching_words[i];
      		// replace characters when ciphetext character match
			for(int i=0;i<cipher_scheme.dict_words[index].size();i++)
			{
				cipher_scheme = replace_keys(cipher_scheme.cipher_num[index][i], cipher_scheme.dict_words[index][i], cipher_scheme);
			}
      		// check if the encryption scheme is complete
			if(this->check_en_scheme_completion(cipher_scheme.dict_words))
			{
        		// final time for decryption
				this->total_time = float(clock() - this->begin_time)/CLOCKS_PER_SEC;
        		// print the decrypted plaintext
				this->print_decrypted_output(cipher_scheme.dict_words, cipher_scheme.cipher_num);
			}
			else
			{
        		// recursively complete the unfilled characters of the ciphertext
				process_cipher_text(cipher_scheme);
			}
		}
	}

	void print_decrypted_output(vector<string> dict_words, vector< vector<int> >cipher_num)
	{
		cout<<endl<<endl;
		cout<<"The decrypted plaintext guess is : "<<endl;
		for(int i=0;i<dict_words.size();i++)
		{
			cout<<dict_words[i]<<" ";
		}
		cout<<endl<<endl;
	}

  	// check if encryption map is all filled out
	bool check_en_scheme_completion(vector<string> dict_words)
	{
		for(int i=0;i<dict_words.size();i++)
		{
			for(int j=0;j<dict_words[i].size();j++)
			{
        		// if ciphertext characters unfilled
				if(dict_words[i][j] == '-')
					return false;
			}
		}
		return true;
	}
};
// class ends here

//main function
int main()
{
	string cipher_text;
	int test_number;
  	cout<<"Enter the ciphertext: "<<endl;
  	getline(cin, cipher_text);
  	cout<<"\nEnter the test number:\n 1. plaintext_dictionary\n 2. english_words\n Your input: ";
	cin>> test_number;
  	CryptAnalysis inst(cipher_text, test_number);
  	inst.decryption();
  	return 0;
}

//End of program
