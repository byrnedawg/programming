{"filter":false,"title":"bobbys_Hobbies.sql","tooltip":"/sql/bobbys_Hobbies.sql","undoManager":{"mark":0,"position":0,"stack":[[{"start":{"row":0,"column":0},"end":{"row":38,"column":24},"action":"insert","lines":["CREATE TABLE persons (","    id INTEGER PRIMARY KEY AUTOINCREMENT,","    name TEXT,","    age INTEGER);","    ","INSERT INTO persons (name, age) VALUES (\"Bobby McBobbyFace\", 12);","INSERT INTO persons (name, age) VALUES (\"Lucy BoBucie\", 25);","INSERT INTO persons (name, age) VALUES (\"Banana FoFanna\", 14);","INSERT INTO persons (name, age) VALUES (\"Shish Kabob\", 20);","INSERT INTO persons (name, age) VALUES (\"Fluffy Sparkles\", 8);","","CREATE table hobbies (","    id INTEGER PRIMARY KEY AUTOINCREMENT,","    person_id INTEGER,","    name TEXT);","    ","INSERT INTO hobbies (person_id, name) VALUES (1, \"drawing\");","INSERT INTO hobbies (person_id, name) VALUES (1, \"coding\");","INSERT INTO hobbies (person_id, name) VALUES (2, \"dancing\");","INSERT INTO hobbies (person_id, name) VALUES (2, \"coding\");","INSERT INTO hobbies (person_id, name) VALUES (3, \"skating\");","INSERT INTO hobbies (person_id, name) VALUES (3, \"rowing\");","INSERT INTO hobbies (person_id, name) VALUES (3, \"drawing\");","INSERT INTO hobbies (person_id, name) VALUES (4, \"coding\");","INSERT INTO hobbies (person_id, name) VALUES (4, \"dilly-dallying\");","INSERT INTO hobbies (person_id, name) VALUES (4, \"meowing\");","","INSERT INTO persons (name, age) VALUES (\"Greg Byrne\", 28);","INSERT INTO hobbies (person_id, name) VALUES (6, \"meowing\");","","SELECT persons.name, hobbies.name FROM persons","    JOIN hobbies","    ON persons.id = hobbies.person_id;","    ","SELECT persons.name, hobbies.name FROM persons","    JOIN hobbies","    ON persons.id = hobbies.person_id","    WHERE persons.name = ","    \"Bobby McBobbyFace\";"],"id":1}]]},"ace":{"folds":[],"scrolltop":0,"scrollleft":0,"selection":{"start":{"row":38,"column":24},"end":{"row":38,"column":24},"isBackwards":false},"options":{"guessTabSize":true,"useWrapMode":false,"wrapToView":true},"firstLineState":0},"timestamp":1482868425258,"hash":"38de685a9e4198f07c7ff447f64c30e0c8537367"}