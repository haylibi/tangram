CREATE DATABASE postgis;
\connect postgis;

CREATE SCHEMA postgis;
ALTER DATABASE postgis SET search_path=public, postgis, contrib;
\connect postgis;

CREATE EXTENSION postgis SCHEMA postgis;


DROP TABLE IF EXISTS piece;
CREATE TABLE piece (
    piece_id SERIAL PRIMARY KEY,
    piece_type VARCHAR(64),
    shape GEOMETRY(POLYGON),
    color VARCHAR(255)
);


DROP TABLE IF EXISTS puzzle;
CREATE TABLE puzzle (
    id SERIAL PRIMARY KEY,
    name VARCHAR(64),
    shape GEOMETRY(POLYGON),
    color VARCHAR(255)
);
-- think on how to get multiple pieces and colors
DROP TABLE IF EXISTS solution;
CREATE TABLE solution (
    id SERIAL PRIMARY KEY,
    puzzle_name VARCHAR(64),
    name VARCHAR(64),
    shape GEOMETRY(POLYGON),
    color VARCHAR(255)
);

INSERT INTO piece (piece_type, shape, color) VALUES
  ('Large Triangle 1', ST_GeomFromText('POLYGON((0 0, 4 0, 0 4, 0 0))'),'red'),
  ('Large Triangle 2', ST_GeomFromText('POLYGON((0 0, 4 0, 0 4, 0 0))'),'orange'),
  ('Medium Triangle', ST_GeomFromText('POLYGON((0 0, 4 0, 2 2, 0 0))'),'blue'),
  ('Small Triangle 1', ST_GeomFromText('POLYGON((0 0, 2 0, 0 2, 0 0))'),'purple'),
  ('Small Triangle 2', ST_GeomFromText('POLYGON((0 0, 2 0, 0 2, 0 0))'),'pink'),
  ('Square', ST_GeomFromText('POLYGON((0 0, 2 0, 2 2, 0 2, 0 0))'),'yellow'),
  ('Parallelogram', ST_GeomFromText('POLYGON((0 0, 2 0, 4 2, 2 2, 0 0))'),'green');


INSERT INTO puzzle (name, shape, color) VALUES
('Puzzle 1', ST_GeomFromText('POLYGON((1.172 0.0, 2.586 1.415, 1.172 2.829, 2.586 4.243, 0.0 6.829, 8.0 6.829, 5.415 4.243, 6.829 2.829, 5.415 1.415, 6.829 0.0, 1.172 0.0))'),'black'),
('Puzzle 2', ST_GeomFromText('POLYGON((1.172 0.0, 2.586 1.415, 1.172 2.829, 2.586 4.243, 0.0 6.829, 2.829 6.829, 4.243 5.415, 5.657 6.829, 8.0 6.829, 5.415 4.243, 6.829 2.829, 5.415 1.415, 6.829 0.0, 1.172 0.0))'),'black'),
('Puzzle 3', ST_GeomFromText('POLYGON((1.0 0.0, 7.0 0.0, 5.415 1.586, 6.829 3.0, 5.415 4.415, 8.0 7.0, 0.0 7.0, 2.586 4.415, 1.172 3.0, 2.586 1.586, 1.0 0.0), (4.0 3.0, 5.0 2.0, 3.0 2.0, 4.0 3.0))'),'black');


INSERT INTO solution (puzzle_name, name, shape, color) VALUES

  ('Puzzle 1', 'Large Triangle 1', ST_GeomFromText('POLYGON((0.0 6.829, 4.0 6.829, 4.0 2.829, 0.0 6.829))'),'red'),
  ('Puzzle 1', 'Large Triangle 2', ST_GeomFromText('POLYGON((4.0 6.829, 8.0 6.829, 4.0 2.829, 4.0 6.829))'),'orange'),
  ('Puzzle 1', 'Medium Triangle',  ST_GeomFromText('POLYGON((2.586 1.415, 5.415 4.243, 5.415 1.415, 2.586 1.415))'),'blue'),
  ('Puzzle 1', 'Small Triangle 1', ST_GeomFromText('POLYGON((1.172 0.0, 4.0 0.0, 2.586 1.415, 1.172 0.0))'),'purple'),
  ('Puzzle 1', 'Small Triangle 2', ST_GeomFromText('POLYGON((5.415 4.243, 6.829 2.829, 5.415 1.415, 5.415 4.243))'),'pink'),
  ('Puzzle 1', 'Square',           ST_GeomFromText('POLYGON((1.172 2.829, 2.586 1.415, 4.0 2.829, 2.586 4.243, 1.172 2.829))'),'yellow'),
  ('Puzzle 1', 'Parallelogram',    ST_GeomFromText('POLYGON((4.0 0.0, 6.829 0.0, 5.415 1.415, 2.586 1.415, 4.0 0.0))'),'green'),



  ('Puzzle 2', 'Large Triangle 1', ST_GeomFromText('POLYGON((1.415 0.0, 7.072 0.0, 4.243 2.829, 1.415 0.0))'),'red'),
  ('Puzzle 2', 'Large Triangle 2', ST_GeomFromText('POLYGON((5.657 1.415, 5.657 7.072, 2.829 4.243, 5.657 1.415))'),'orange'),
  ('Puzzle 2', 'Medium Triangle',  ST_GeomFromText('POLYGON((5.657 7.072, 8.486 7.072, 5.657 4.243, 5.657 7.072))'),'blue'),
  ('Puzzle 2', 'Small Triangle 1', ST_GeomFromText('POLYGON((5.657 4.243, 7.072 2.829, 5.657 1.415, 5.657 4.243))'),'purple'),
  ('Puzzle 2', 'Small Triangle 2', ST_GeomFromText('POLYGON((1.415 5.657, 4.243 5.657, 2.829 4.243, 1.415 5.657))'),'pink'),
  ('Puzzle 2', 'Square',           ST_GeomFromText('POLYGON((1.415 2.829, 2.829 1.415, 4.243 2.829, 2.829 4.243, 1.415 2.829))'),'yellow'),
  ('Puzzle 2', 'Parallelogram',    ST_GeomFromText('POLYGON((0.0 7.072, 1.415 5.657, 4.243 5.657, 2.829 7.072, 0.0 7.072))'),'green'),

  ('Puzzle 3', 'Large Triangle 1', ST_GeomFromText('POLYGON((0.0 7.0, 4.0 7.0, 4.0 3.0, 0.0 7.0))'),'red'),
  ('Puzzle 3', 'Large Triangle 2', ST_GeomFromText('POLYGON((4.0 7.0, 8.0 7.0, 4.0 3.0, 4.0 7.0))'),'orange'),
  ('Puzzle 3', 'Medium Triangle',  ST_GeomFromText('POLYGON((3.0 0.0, 7.0 0.0, 5.0 2.0, 3.0 0.0))'),'blue'),
  ('Puzzle 3', 'Small Triangle 1', ST_GeomFromText('POLYGON((4.0 3.0, 5.415 1.586, 6.829 3.0, 4.0 3.0))'),'purple'),
  ('Puzzle 3', 'Small Triangle 2', ST_GeomFromText('POLYGON((4.0 3.0, 6.829 3.0, 5.415 4.415, 4.0 3.0))'),'pink'),
  ('Puzzle 3', 'Square',           ST_GeomFromText('POLYGON((4.0 3.0, 2.586 4.415, 1.172 3.0, 2.586 1.586, 4.0 3.0))'),'yellow'),
  ('Puzzle 3', 'Parallelogram',    ST_GeomFromText('POLYGON((1.0 0.0, 3.0 0.0, 5.0 2.0, 3.0 2.0, 1.0 0.0))'),'green');








