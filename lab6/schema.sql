-- Схема БД для простой системы библиотеки
-- Тема: библиотека (книги, авторы, читатели, выдачи, жанры)

DROP TABLE IF EXISTS borrowings CASCADE;
DROP TABLE IF EXISTS books CASCADE;
DROP TABLE IF EXISTS authors CASCADE;
DROP TABLE IF EXISTS genres CASCADE;
DROP TABLE IF EXISTS readers CASCADE;

-- 1. Авторы
CREATE TABLE authors (
    author_id SERIAL PRIMARY KEY,
    name      TEXT NOT NULL
);

-- 2. Жанры
CREATE TABLE genres (
    genre_id SERIAL PRIMARY KEY,
    name     TEXT NOT NULL UNIQUE
);

-- 3. Книги
CREATE TABLE books (
    book_id     SERIAL PRIMARY KEY,
    title       TEXT NOT NULL,
    author_id   INT  NOT NULL REFERENCES authors(author_id) ON DELETE CASCADE,
    genre_id    INT  NOT NULL REFERENCES genres(genre_id)  ON DELETE SET NULL,
    total_copies INT NOT NULL CHECK (total_copies >= 0)
);

-- 4. Читатели
CREATE TABLE readers (
    reader_id SERIAL PRIMARY KEY,
    full_name TEXT NOT NULL,
    email     TEXT UNIQUE
);

-- 5. Выдачи книг
CREATE TABLE borrowings (
    borrowing_id SERIAL PRIMARY KEY,
    reader_id    INT NOT NULL REFERENCES readers(reader_id) ON DELETE CASCADE,
    book_id      INT NOT NULL REFERENCES books(book_id)     ON DELETE CASCADE,
    borrow_date  DATE NOT NULL,
    return_date  DATE
);

-- Простейшие начальные данные
INSERT INTO authors (name) VALUES
('Фёдор Достоевский'),
('Лев Толстой'),
('Александр Пушкин');

INSERT INTO genres (name) VALUES
('Роман'),
('Поэзия'),
('Повесть');

INSERT INTO readers (full_name, email) VALUES
('Иван Иванов',  'ivan@example.com'),
('Пётр Петров',  'petr@example.com'),
('Мария Смирнова', 'maria@example.com');

INSERT INTO books (title, author_id, genre_id, total_copies) VALUES
('Преступление и наказание', 1, 1, 5),
('Война и мир',               2, 1, 3),
('Евгений Онегин',            3, 2, 4);

INSERT INTO borrowings (reader_id, book_id, borrow_date, return_date) VALUES
(1, 1, '2025-01-10', '2025-01-20'),
(2, 1, '2025-02-01', NULL),
(3, 2, '2025-03-05', NULL),
(1, 3, '2025-03-10', '2025-03-25');


