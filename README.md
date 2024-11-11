Anthony Jirano jirano@csu.fullerton.edu
Dmitry Bozhko dbozhko@csu.fullerton.edu
Emily Nguyen enguyen0401@csu.fullerton.edu
Kaitlyn Lee kaitlynlee@csu.fullerton.edu

To compile and run the program, open your terminal, navigate to the directory containing the files from this GitHub, and type: g++ 335project2.cpp—o 335project2. Then, run it by typing ./335project2 on Mac/Linux or 335project2.exe on Windows (if you are on Windows, change the second thing from 335project2 to 335project2.exel).
Then to run the program with inputted user created numbers first run the code in your desired directory. Once you run the code it will create a file named input.txt and output.txt in the code's directory.
Open the input.txt file and it will be populated with 10 test cases. You can choose to modify any of these test cases or remove them to add your own using this format:
2
30
09:00 18:00
09:00 10:30, 12:00 13:00, 15:00 16:00
09:00 17:00
10:00 11:30, 13:00 14:00, 16:00 17:00
In the first line we see the number 2. Replace this number with the number of people for the meeting (maximum 5 people).
Next number is 30. Replace this number for the length of the meeting in minutes.
Every line after this will be in pairs for example lines:
09:00 18:00
09:00 10:30, 12:00 13:00, 15:00 16:00
Are a pair
09:00 17:00
10:00 11:30, 13:00 14:00, 16:00 17:00
Are a pair.
These are to represent people's available time and busy time respectively.
We'll be looking at the first set of numbers in the first pair: 09:00 18:00
This number represents the range of person one's available working time. Please replace these numbers in 24 hr format with a space in between as per shown in the example.
The second set of numbers in the first pair 09:00 10:30, 12:00 13:00, 15:00 16:00
Are the busy times that don't work for person one. Replace these numbers in 24 hr format with a comma in between each range. You may add as many busy times as needed as long as you add a comma in between. If the person has no busy times, you can leave this line empty.
Person two will be the next pair of numbers. Example in the file generated person two's active range is 09:00 17:00. Replace these numbers in 24 hr format. Steps following will be the same as for person one.
If the first line in the text file has a number greater than 2 (up to 5), to represent the amount of people's schedule we'll be working with, then simply keep adding pairs of numbers as done before.
You can add multiple test cases in the file, just make sure to follow the same format for each test case.
Save and close file.
Run code again and it will grab the numbers from the input.txt and will show available time slots that work for everyone in a file named output.txt.

