CREATE TABLE gamer( -- шардируется по team_id
  id INTEGER PRIMARY KEY,
  team_id INTEGER,
  nikname TEXT,
  full_name TEXT,
  description TEXT
);

CREATE TABLE team( -- шардируется по id
  id INTEGER PRIMARY KEY,
  name TEXT,
  description TEXT
);

-- маленькая таблица с описанием названий игр
CREATE TABLE game( -- копируется на все шарды
   id INTEGER PRIMARY KEY,
   name TEXT,
   developer TEXT,
   description TEXT
);

CREATE TABLE match ( -- шардируется по tourney_id
  id INTEGER PRIMARY KEY,
  tourney_id INTEGER,
  team_1_id INTEGER,
  team_2_id INTEGER,
  score_1_team INTEGER,
  score_2_team INTEGER,
  description TEXT,
  broadcast_link TEXT
);

CREATE TABLE tourney( -- шардируется по id
  id INTEGER PRIMARY KEY,
  game_id INTEGER,
  description TEXT
);

CREATE PUBLICATION publication_tables1; -- for master1
ALTER PUBLICATION publication_tables1 ADD TABLE
  gamer,
  team,
  game,
  match,
  tourney;

CREATE SUBSCRIPTION subscription_tables1 -- for slave1
  CONNECTION 'host=192.168.1.13 port=5401 user=postgres dbname=postgres'
  PUBLICATION publication_tables1;

CREATE PUBLICATION publication_tables2; -- for master2
ALTER PUBLICATION publication_tables1 ADD TABLE
  gamer,
  team,
  game,
  match,
  tourney;

CREATE SUBSCRIPTION subscription_tables2 -- for slave2
  CONNECTION 'host=192.168.1.151 port=5404 user=postgres dbname=postgres'
  PUBLICATION publication_tables2;

-- для добавления тестовых данных.
INSERT INTO gamer(id, team_id, nikname, full_name, description) VALUES
  (1, 1, 'RodjER', 'Владимира Никогосяна', 'Недавно перешёл в команду'),
  (2, 2, 'Lil', 'Ильи Ильюка', 'Недавно перешёл в команду');
INSERT INTO team (id, name, description) VALUES
  (1, 'Natus Vincere', 'Единственная команда о которой я слышал. Друг увлекался ставками на киберспорт.'),
  (2, 'Virtus.pro', 'Довольно сильная команда. Оказывается, за игроками стоят крупные корпорации. Реклама во время трансляций.');
INSERT INTO game (id, name, developer, description) VALUES
  (1, 'Dota', 'valve', 'Самая задротская игра последнего десятилетия');
INSERT INTO tourney(id, game_id, description) VALUES
  (1, 1, 'Chf;tybt');
INSERT INTO match (id, tourney_id, team_1_id, team_2_id, score_1_team, score_2_team, description, broadcast_link) VALUES
  (1, 1, 1, 2, 5, 6, 'потная игра', 'https://www.youtube.com/');


-- для извлечения дданных.
SELECT id, team_id, nikname, full_name, description FROM gamer WHERE team_id = ?;
SELECT id, name, description FROM team WHERE id = ?;
INSERT INTO game (id, name, developer, description) VALUES
  (1, 'Dota', 'valve', 'Самая задротская игра последнего десятилетия');
INSERT INTO tourney(id, game_id, description) VALUES
  (1, 1, 'Chf;tybt');
INSERT INTO match (id, tourney_id, team_1_id, team_2_id, score_1_team, score_2_team, description, broadcast_link) VALUES
  (1, 1, 1, 2, 5, 6, 'потная игра', 'https://www.youtube.com/');
