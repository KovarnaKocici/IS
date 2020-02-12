/*
Бейрак Євгенія, ТТП-42
Предметна область: Мої улюблені фільми. 

Відношення-факти: 
Фільм (назва, режисер, рік випуску, анотація). 
Актор (ПІБ, звання, рік народження, місце роботи). 
Знімався (назва фільму, ПІБ, роль, характеристика ролі). Під характеристикою ролі розуміється: головна, епізодична,…. 
*/

/*Структури*/

starred(
film('The Prestige','Christopher Nolan',2006,'Two stage magicians engage in competitive one-upmanship in an attempt to create the ultimate stage illusion.'),
actor('Christian Bale','actor',1974,'Hollywood'),
'Alfred Borden',
main).

starred(
film('Inception','Christopher Nolan',2010,'A thief, who steals corporate secrets through use of dream-sharing technology, is given the inverse task of planting an idea into the mind of a CEO.'),
actor('Leonardo DiCaprio','actor',1974,'Hollywood'),
'Cobb',
main).

starred(
film('Inception','Christopher Nolan',2010,'A thief, who steals corporate secrets through use of dream-sharing technology, is given the inverse task of planting an idea into the mind of a CEO.'),
actor('Joseph Gordon-Levitt','actor',1981,'Hollywood'),
'Arthur',
secondary).

starred(
film('The Dark Knight Rises','Christopher Nolan',2012,'Eight years after the Jokers reign of anarchy, the Dark Knight, with the help of the enigmatic Selina, is forced from his imposed exile to save Gotham City, now on the edge of total annihilation, from the brutal guerrilla terrorist Bane.'),
actor('Christian Bale','actor',1974,'Hollywood'),
'Bruce Wayne',
main).

starred(
film('The Dark Knight Rises','Christopher Nolan',2012,'Eight years after the Jokers reign of anarchy, the Dark Knight, with the help of the enigmatic Selina, is forced from his imposed exile to save Gotham City, now on the edge of total annihilation, from the brutal guerrilla terrorist Bane.'),
actor('Joseph Gordon-Levitt','actor',1981,'Hollywood'),
'Blake',
secondary).

starred(
film('(500) Days of Summer','Marc Webb',2009,'An offbeat romantic comedy about a woman who does not believe true love exists, and the young man who falls for her.'),
actor('Joseph Gordon-Levitt','actor',1981,'Hollywood'),
'Tom',
main).

starred(
film('Shutter Island','Martin Scorsese',2010,'In 1954, a U.S. marshal investigates the disappearance of a murderess who escaped from a hospital for the criminally insane.'),
actor('Leonardo DiCaprio','actor',1974,'Hollywood'),
'Teddy Daniels',
main).


/*Відношення-правила*/


/*В яких фільмах знімався даний актор*/

/*Запит для будь-якого актора (назву вводити з командної стрічки).
Наприклад: req21('Leonardo DiCaprio',Y).*/
req21(X,Y):-starred(film(Y,_,_,_),actor(X,_,_,_),_,_).

/*В яких фільмах знімався Leonardo DiCaprio*/
req211(Y):-starred(film(Y,_,_,_),actor('Leonardo DiCaprio',_,_,_),_,_).


/*Вік акторів, зайнятих на зйомках даного фільму*/

/*Запит для будь-якого фільму
Наприклад: req22('Inception',X,Y).*/
req22(A,X,Y):-starred(film(A,_,_,_),actor(X,_,Z,_),_,_),Y is (2017-Z).

/*Вік акторів, зайнятих на зйомках фільму 'Inception'*/
req222(X,Y):-starred(film('Inception',_,_,_),actor(X,_,Z,_),_,_),Y is (2017-Z).


/*Xто з акторів зіграв у заданому фільмі вказану роль*/

/*Запит для будь-якого фільму та будь-якої ролі
Наприклад: req23('The Dark Knight Rises','Blake',Y).*/
req23(X,Z,Y):-starred(film(X,_,_,_),actor(Y,_,_,_),Z,_).

/*Хто з акторів зіграв у 'The Dark Knight Rises' роль 'Blake'*/
req233(Y):-starred(film('The Dark Knight Rises',_,_,_),actor(Y,_,_,_),'Blake',_).


/*Хто режисер фільму, у якому певний актор зіграв задану роль*/

/*Запит для будь-якого актора та будь-якої ролі
Наприклад: req24('Christian Bale','Alfred Borden',Y).*/
req24(A,B,Y):-starred(film(_,Y,_,_),actor(A,_,_,_),B,_).

/*Режисер фільму, у якому Christian Bale зіграв роль 'Alfred Borden'*/
req244(Y):-starred(film(_,Y,_,_),actor('Christian Bale',_,_,_),'Alfred Borden',_).


/*Назви фільмів, у яких грає певний актор та які вийшли у заданому році*/

/*Запит для будь-якого актора та будь-якого року
Наприклад: req25('Joseph Gordon-Levitt',2009,Y).*/
req25(A,B,Y):-starred(film(Y,_,B,_),actor(A,_,_,_),_,_).

/*Назви фільмів, у яких грає Joseph Gordon-Levitt та які вийшли у 2009 році*/
req255(Y):-starred(film(Y,_,2009,_),actor('Joseph Gordon-Levitt',_,_,_),_,_).