#----------------------------------------------------------
#Cryptanalysis of Homophonic Ciphers
#----------------------------------------------------------

import random
import re

# ---------------------
# Make frequency table
# ---------------------

out = open('testcase.txt', "w")

freq = {}
with open('frequency.txt', 'r') as f:
	data = f.read()
	data = data.split('\n')

for datum in data[:len(data)-1]:
	datum = datum.split()
	key, value = datum[0], datum[1]
	freq[key] = value

used = []
col3 = {}
for key in freq:
	f = int(freq[key])
	key_list = []
	while f>0:
		val = random.randint(0, 102)
		while val in used:
			val = random.randint(0, 102)

		used.append(val)
		key_list.append(val)
		f = f - 1

	col3[key] = key_list

for key in col3:
	print "%s: %s" %(key, col3[key])
	out.write("%s: %s\n" %(key, col3[key]))


# ----------------------------
# Generate a random plaintext
# ----------------------------

with open('plaintext_dictionary.txt', 'r') as f:
	data = f.read()
	#data = data.split()
	#data = re.split('\n|\s',data)
	data = re.split('\n|\s',data)
	data = filter(None, data)

wordlist = data[:len(data)-1]

#num_of_words = random.randint(1, 5)
num_of_words = 20
plain_text = ''
while num_of_words>0:
	random_word = wordlist[random.randint(0, 500)]
	plain_text = plain_text + random_word + ' '
	num_of_words = num_of_words - 1

plain_text = plain_text[:len(plain_text)-1]
print plain_text
out.write(plain_text)
out.write('\n')


# -------------------------------------
# Encrypt randomly generated plaintext
# -------------------------------------

ptext_list = list(plain_text)
cipher_text = ''
for i, c in enumerate(ptext_list):
	if c == ' ':
		cipher_text = cipher_text + c
	else:
		list_length = int(freq[c])
		pos = i%list_length
		cipher_c = col3[c][pos]
		cipher_text = cipher_text + str(cipher_c) + ','

print cipher_text
out.write(cipher_text)

out.close()
