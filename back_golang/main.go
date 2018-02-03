package main

import (
	_ "github.com/lib/pq"
	"database/sql"
	"fmt"
	"errors"
)

/*CREATE TABLE gamer( -- шардируется по team_id
  id INTEGER PRIMARY KEY,
  team_id INTEGER,
  nikname TEXT,
  full_name TEXT,
  description TEXT
);*/

type gamer struct {
	id          int
	team_id     int
	nikname     string
	full_name   string
	description string
}

/*CREATE TABLE team( -- шардируется по id
  id INTEGER PRIMARY KEY,
  name TEXT,
  description TEXT
);*/

type team struct {
	id          int
	name        string
	description string
}

/*-- маленькая таблица с описанием названий игр
CREATE TABLE game( -- копируется на все шарды
   id INTEGER PRIMARY KEY,
   name TEXT,
   developer TEXT,
   description TEXT
);*/

type game struct {
	id          int
	name        string
	developer   string
	description string
}

/*CREATE TABLE match ( -- шардируется по tourney_id
  id INTEGER PRIMARY KEY,
  tourney_id INTEGER,
  team_1_id INTEGER,
  team_2_id INTEGER,
  score_1_team INTEGER,
  score_2_team INTEGER,
  description TEXT,
  broadcast_link TEXT
);*/

type match struct {
	id             int
	tourney_id     int
	team_1_id      int
	team_2_id      int
	score_1_team   int
	score_2_team   int
	description    string
	broadcast_link string
}

/*CREATE TABLE tourney( -- шардируется по id
  id INTEGER PRIMARY KEY,
  game_id INTEGER,
  description TEXT
);*/

type tourney struct {
	id          int
	game_id     int
	description string
}

// класс шардирования
// держит 2 соединения
// держит шардирующую функцию
type СonnectionsToDatabase struct {
	//192.168.1.151:5401 winter-db2-student-7 master1
	connect_1 *sql.DB
	connect_1_is_alive bool
	//192.168.1.151:5402 winter-db2-student-7 slave2
	connect_2 *sql.DB
	connect_2_is_alive bool
	//192.168.1.13:5403  winter-db1-student-7 slave1
	connect_3 *sql.DB
	connect_3_is_alive bool
	//192.168.1.13:5404  winter-db1-student-7 master2
	connect_4 *sql.DB
	connect_4_is_alive bool
}

func (this *СonnectionsToDatabase) Open () error {
	//192.168.1.151:5401 winter-db2-student-7 master1
	url_1 := "postgres://postgres:postgres@/192.168.1.151:5401"
	connect_1, err := sql.Open("postgres", url_1)
	if err != nil {
		return errors.New("Невозможно установить соединение: " + url_1 + err.Error())
	} else {
		this.connect_1 = connect_1
		this.connect_1_is_alive = true
	}

	//192.168.1.151:5402 winter-db2-student-7 slave2
	url_2 := "postgres://postgres:postgres@//192.168.1.151:5402"
	connect_2, err := sql.Open("postgres", url_2)
	if err != nil {
		return errors.New("Невозможно установить соединение: " + url_2 + err.Error())
	} else {
		this.connect_2 = connect_2
		this.connect_2_is_alive = true
	}

	//192.168.1.13:5403  winter-db1-student-7 slave1
	url_3 := "postgres://postgres:postgres@192.168.1.13:5403"
	connect_3, err := sql.Open("postgres", url_3)
	if err != nil {
		return errors.New("Невозможно установить соединение: " + url_3 + err.Error())
	} else {
		this.connect_3 = connect_3
		this.connect_3_is_alive = true
	}

	//192.168.1.13:5404  winter-db1-student-7 master2
	url_4 := "postgres://postgres:postgres@192.168.1.13:5404"
	connect_4, err := sql.Open("postgres", url_4)
	if err != nil {
		return errors.New("Невозможно установить соединение: " + url_4 + err.Error())
	} else {
		this.connect_4 = connect_4
		this.connect_4_is_alive = true
	}
	return nil
}

func main() {
	//создаём пулл соединений
	connections_to_database := СonnectionsToDatabase{}
	err := connections_to_database.Open()
	if err != nil {
		panic(err)
	} else {
		fmt.Print("Соединение с базами установлено")
	}

}


// придумать api
// придумать типовые запросы

// Страница команды должна содержать список своих игроков и, опционально,
// список последних 5 турниров, на которых участвовала.
// запрос информации о команде по id
// запрос списка игроков по team_id


// Турниры должны содержать
// список команд (участников),
// информацию об этапах,
// даты проведения и
// расписание матчей.
// запрос к таблице турниров по id
// запрос к таблице матчей по tourney_id


// Матчи должны содержать
// результаты,
// составы команд и, опционально,
// ссылки на видеоматериалы.
// запрос матча по id
// запрос команд по вытащенному team_id