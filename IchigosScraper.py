import requests
import bs4
import os
import re
from string import ascii_lowercase

website = 'https://ichigos.com'
base_dir = 'C:\\Users\\nickb\\Downloads\\Ichigos'

for letter in list(ascii_lowercase) + ['#', 'fi']:
	res = requests.get(website + '/sheets/' + letter)
	res.raise_for_status()

	soup = bs4.BeautifulSoup(res.text, features="lxml")

	elem = soup.find('span', class_='title2')
	while elem is not None:
		title = elem.text.strip().replace(" ", "_")
		title = re.sub(r'\W+', '', title)
		print(title)

		elem = elem.next_sibling
		while elem is not None and elem.name != 'span':
			elem = elem.next_sibling

			if elem and elem.name == 'i':
				instruments = elem.text.lower()

				if 'piano' in instruments and \
					'piano,' not in instruments and \
					', piano' not in instruments and \
					'piano and' not in instruments and \
					'and piano' not in instruments:

					song = elem.previous_sibling.previous_sibling.previous_sibling
					song = song.string.strip().replace(" ", "_")
					song = re.sub(r'\W+', '', song)
					#print(title)
					print('\t' + song)

					elem = elem.next_sibling.next_sibling
					while elem.name == 'a':
						file_type = elem.text
						url_resource = elem['href']
						print('\t\t' + file_type, end=" ")
						print(url_resource)

						if file_type == 'pdf' or file_type == 'midi':
							file_content = requests.get(website + url_resource)
							file_content.raise_for_status()

							directory = os.path.join(base_dir, title)
							if not os.path.exists(directory):
								os.makedirs(directory)

							if file_type == 'pdf':
								file_name = os.path.join(directory, song + '.pdf')
							elif file_type == 'midi':
								file_name = os.path.join(directory, song + '.mid')
							print(file_name)
							file = open(file_name, 'wb')
							for chunk in file_content.iter_content(100000):
								file.write(chunk)
							file.close()

						elem = elem.next_sibling.next_sibling
