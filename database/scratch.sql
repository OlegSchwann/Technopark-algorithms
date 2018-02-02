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
ALTER PUBLICATION tables1 ADD TABLE
  gamer,
  team,
  game,
  match,
  tourney;

CREATE SUBSCRIPTION subscription_tables1 -- for slave1
  CONNECTION 'host=192.168.1.13 port=5401 user=postgres dbname=postgres'
  PUBLICATION publication_tables1;

