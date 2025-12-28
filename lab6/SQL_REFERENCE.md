# Справочник по SQL-командам для PostgreSQL

## Содержание
1. [Основные команды](#основные-команды)
2. [JOIN операции](#join-операции)
3. [Условия WHERE и HAVING](#условия-where-и-having)
4. [Группировка и агрегатные функции](#группировка-и-агрегатные-функции)
5. [Подзапросы](#подзапросы)
6. [Объединение результатов](#объединение-результатов)
7. [Работа с NULL](#работа-с-null)
8. [Транзакции](#транзакции)

---

## Основные команды

### SELECT - выборка данных

**Синтаксис:**
```sql
SELECT столбец1, столбец2, ...
FROM таблица
[WHERE условие]
[ORDER BY столбец [ASC|DESC]]
[LIMIT количество];
```

**Примеры:**
```sql
-- Выбрать все столбцы из таблицы
SELECT * FROM books;

-- Выбрать конкретные столбцы
SELECT title, author_id FROM books;

-- Выбрать с условием
SELECT * FROM books WHERE total_copies > 3;

-- Выбрать с сортировкой
SELECT * FROM books ORDER BY title ASC;

-- Ограничить количество результатов
SELECT * FROM books LIMIT 5;
```

### INSERT - вставка данных

**Синтаксис:**
```sql
INSERT INTO таблица (столбец1, столбец2, ...)
VALUES (значение1, значение2, ...);
```

**Примеры:**
```sql
-- Вставить одну запись
INSERT INTO authors (name) VALUES ('Антон Чехов');

-- Вставить несколько записей
INSERT INTO books (title, author_id, genre_id, total_copies)
VALUES 
    ('Анна Каренина', 2, 1, 3),
    ('Капитанская дочка', 3, 3, 2);
```

### UPDATE - обновление данных

**Синтаксис:**
```sql
UPDATE таблица
SET столбец1 = значение1, столбец2 = значение2, ...
WHERE условие;
```

**Примеры:**
```sql
-- Обновить одну запись
UPDATE books SET total_copies = 5 WHERE book_id = 1;

-- Обновить несколько столбцов
UPDATE readers SET full_name = 'Иван Петров', email = 'ivan@mail.ru'
WHERE reader_id = 1;

-- Обновить с арифметикой
UPDATE books SET total_copies = total_copies - 1 WHERE book_id = 1;
```

### DELETE - удаление данных

**Синтаксис:**
```sql
DELETE FROM таблица
WHERE условие;
```

**Примеры:**
```sql
-- Удалить конкретную запись
DELETE FROM books WHERE book_id = 5;

-- Удалить все записи (осторожно!)
DELETE FROM borrowings WHERE return_date IS NOT NULL;
```

---

## JOIN операции

### INNER JOIN - внутреннее соединение

**Синтаксис:**
```sql
SELECT столбцы
FROM таблица1
INNER JOIN таблица2 ON таблица1.ключ = таблица2.ключ;
```

**Пример:**
```sql
-- Книги с авторами
SELECT b.title, a.name AS author
FROM books b
INNER JOIN authors a ON b.author_id = a.author_id;
```

### LEFT JOIN - левое внешнее соединение

**Синтаксис:**
```sql
SELECT столбцы
FROM таблица1
LEFT JOIN таблица2 ON таблица1.ключ = таблица2.ключ;
```

**Пример:**
```sql
-- Все читатели и их выдачи (даже если выдачи нет)
SELECT r.full_name, b.title
FROM readers r
LEFT JOIN borrowings br ON r.reader_id = br.reader_id
LEFT JOIN books b ON br.book_id = b.book_id;
```

### RIGHT JOIN - правое внешнее соединение

**Синтаксис:**
```sql
SELECT столбцы
FROM таблица1
RIGHT JOIN таблица2 ON таблица1.ключ = таблица2.ключ;
```

**Пример:**
```sql
-- Все выдачи и читатели (даже если читатель удалён)
SELECT br.borrow_date, r.full_name
FROM borrowings br
RIGHT JOIN readers r ON br.reader_id = r.reader_id;
```

### FULL JOIN - полное внешнее соединение

**Синтаксис:**
```sql
SELECT столбцы
FROM таблица1
FULL JOIN таблица2 ON таблица1.ключ = таблица2.ключ;
```

**Пример:**
```sql
-- Все книги и все выдачи
SELECT b.title, br.borrow_date
FROM books b
FULL JOIN borrowings br ON b.book_id = br.book_id;
```

### Множественные JOIN

**Пример:**
```sql
-- Соединение трёх таблиц
SELECT b.title, a.name AS author, g.name AS genre
FROM books b
JOIN authors a ON b.author_id = a.author_id
JOIN genres g ON b.genre_id = g.genre_id;
```

---

## Условия WHERE и HAVING

### WHERE - фильтрация строк

**Операторы сравнения:**
- `=` - равно
- `!=` или `<>` - не равно
- `>` - больше
- `<` - меньше
- `>=` - больше или равно
- `<=` - меньше или равно
- `IN` - входит в список
- `NOT IN` - не входит в список
- `LIKE` - поиск по шаблону
- `IS NULL` - является NULL
- `IS NOT NULL` - не является NULL

**Примеры:**
```sql
-- Простое условие
SELECT * FROM books WHERE total_copies > 3;

-- Несколько условий (AND)
SELECT * FROM books 
WHERE author_id = 1 AND genre_id = 1;

-- Несколько условий (OR)
SELECT * FROM books 
WHERE author_id = 1 OR author_id = 2;

-- IN - входит в список
SELECT * FROM books WHERE author_id IN (1, 2, 3);

-- LIKE - поиск по шаблону
SELECT * FROM books WHERE title LIKE '%мир%';

-- IS NULL
SELECT * FROM borrowings WHERE return_date IS NULL;

-- Комбинированные условия
SELECT * FROM books 
WHERE total_copies >= 3 
  AND (author_id = 1 OR author_id = 2)
  AND title IS NOT NULL;
```

### HAVING - фильтрация после группировки

**Синтаксис:**
```sql
SELECT столбец, агрегатная_функция(столбец)
FROM таблица
GROUP BY столбец
HAVING условие_на_агрегат;
```

**Пример:**
```sql
-- Жанры с количеством книг больше 2
SELECT g.name, COUNT(b.book_id) AS book_count
FROM genres g
LEFT JOIN books b ON g.genre_id = b.genre_id
GROUP BY g.name
HAVING COUNT(b.book_id) > 2;
```

**Разница WHERE и HAVING:**
- `WHERE` фильтрует строки ДО группировки
- `HAVING` фильтрует группы ПОСЛЕ группировки

---

## Группировка и агрегатные функции

### GROUP BY - группировка

**Синтаксис:**
```sql
SELECT столбец, агрегатная_функция(столбец)
FROM таблица
GROUP BY столбец;
```

**Пример:**
```sql
-- Количество книг по каждому автору
SELECT a.name, COUNT(b.book_id) AS book_count
FROM authors a
LEFT JOIN books b ON a.author_id = b.author_id
GROUP BY a.author_id, a.name;
```

### Агрегатные функции

**Основные функции:**
- `COUNT(*)` - количество строк
- `COUNT(столбец)` - количество не-NULL значений
- `SUM(столбец)` - сумма
- `AVG(столбец)` - среднее значение
- `MIN(столбец)` - минимальное значение
- `MAX(столбец)` - максимальное значение

**Примеры:**
```sql
-- Общее количество книг
SELECT COUNT(*) AS total_books FROM books;

-- Количество выданных книг по каждому читателю
SELECT r.full_name, COUNT(br.borrowing_id) AS borrowed_count
FROM readers r
LEFT JOIN borrowings br ON r.reader_id = br.reader_id
GROUP BY r.reader_id, r.full_name;

-- Среднее количество экземпляров
SELECT AVG(total_copies) AS avg_copies FROM books;

-- Минимальное и максимальное количество
SELECT MIN(total_copies) AS min_copies, 
       MAX(total_copies) AS max_copies 
FROM books;
```

### ORDER BY - сортировка

**Синтаксис:**
```sql
SELECT столбцы
FROM таблица
ORDER BY столбец1 [ASC|DESC], столбец2 [ASC|DESC];
```

**Примеры:**
```sql
-- Сортировка по возрастанию (ASC по умолчанию)
SELECT * FROM books ORDER BY title;

-- Сортировка по убыванию
SELECT * FROM books ORDER BY total_copies DESC;

-- Сортировка по нескольким столбцам
SELECT * FROM books 
ORDER BY author_id ASC, title DESC;
```

---

## Подзапросы

### Подзапрос в WHERE

**Примеры:**
```sql
-- Книги, которые никогда не выдавались
SELECT title
FROM books
WHERE book_id NOT IN (
    SELECT DISTINCT book_id FROM borrowings
);

-- Читатели с активными выдачами
SELECT full_name
FROM readers
WHERE reader_id IN (
    SELECT reader_id FROM borrowings
    WHERE return_date IS NULL
);

-- Книги с количеством больше среднего
SELECT title, total_copies
FROM books
WHERE total_copies > (
    SELECT AVG(total_copies) FROM books
);
```

### Подзапрос в SELECT

**Пример:**
```sql
-- Количество выданных книг для каждой книги
SELECT 
    b.title,
    (SELECT COUNT(*) 
     FROM borrowings br 
     WHERE br.book_id = b.book_id) AS borrow_count
FROM books b;
```

### EXISTS и NOT EXISTS

**Пример:**
```sql
-- Книги, которые выдавались
SELECT title
FROM books b
WHERE EXISTS (
    SELECT 1 FROM borrowings br
    WHERE br.book_id = b.book_id
);
```

---

## Объединение результатов

### UNION - объединение без дубликатов

**Синтаксис:**
```sql
SELECT столбцы FROM таблица1
UNION
SELECT столбцы FROM таблица2;
```

**Пример:**
```sql
-- Объединить названия книг и имена авторов
SELECT title AS name, 'Книга' AS type FROM books
UNION
SELECT name AS name, 'Автор' AS type FROM authors
ORDER BY type, name;
```

### UNION ALL - объединение с дубликатами

**Пример:**
```sql
SELECT title FROM books WHERE author_id = 1
UNION ALL
SELECT title FROM books WHERE genre_id = 1;
```

---

## Работа с NULL

### Проверка на NULL

```sql
-- Найти записи с NULL
SELECT * FROM borrowings WHERE return_date IS NULL;

-- Найти записи без NULL
SELECT * FROM borrowings WHERE return_date IS NOT NULL;
```

### COALESCE - замена NULL

```sql
-- Заменить NULL на значение по умолчанию
SELECT title, COALESCE(total_copies, 0) AS copies
FROM books;
```

---

## Транзакции

### BEGIN, COMMIT, ROLLBACK

**Синтаксис:**
```sql
BEGIN;
-- SQL команды
COMMIT;  -- или ROLLBACK; для отмены
```

**Пример:**
```sql
BEGIN;
INSERT INTO books (title, author_id, genre_id, total_copies)
VALUES ('Новая книга', 1, 1, 5);
UPDATE books SET total_copies = total_copies - 1 WHERE book_id = 1;
COMMIT;
```

---

## Полезные примеры для лабораторной работы

### 1. Поиск дубликатов

```sql
-- Найти читателей с одинаковыми именами
SELECT r1.reader_id, r1.full_name, r1.email
FROM readers r1
JOIN readers r2 ON r1.full_name = r2.full_name
WHERE r1.reader_id <> r2.reader_id;
```

### 2. Самые популярные книги

```sql
SELECT b.title, COUNT(br.borrowing_id) AS borrow_count
FROM books b
JOIN borrowings br ON b.book_id = br.book_id
GROUP BY b.book_id, b.title
ORDER BY borrow_count DESC
LIMIT 5;
```

### 3. Читатели без активных выдач

```sql
SELECT r.full_name
FROM readers r
WHERE r.reader_id NOT IN (
    SELECT reader_id FROM borrowings
    WHERE return_date IS NULL
);
```

### 4. Статистика по жанрам

```sql
SELECT 
    g.name AS genre,
    COUNT(b.book_id) AS book_count,
    SUM(b.total_copies) AS total_copies
FROM genres g
LEFT JOIN books b ON g.genre_id = b.genre_id
GROUP BY g.genre_id, g.name
HAVING COUNT(b.book_id) > 0
ORDER BY book_count DESC;
```

### 5. Активные выдачи с деталями

```sql
SELECT 
    r.full_name AS reader,
    b.title AS book,
    br.borrow_date,
    (CURRENT_DATE - br.borrow_date) AS days_borrowed
FROM borrowings br
JOIN readers r ON br.reader_id = r.reader_id
JOIN books b ON br.book_id = b.book_id
WHERE br.return_date IS NULL
ORDER BY days_borrowed DESC;
```

---

## Защита от SQL-инъекций

### Параметризованные запросы (в C++)

**Неправильно (уязвимо):**
```cpp
std::string genre = "Роман";
std::string query = "SELECT * FROM books WHERE genre = '" + genre + "'";
```

**Правильно (безопасно):**
```cpp
std::string genre = "Роман";
pqxx::result res = txn.exec(
    "SELECT * FROM books WHERE genre = $1",
    pqxx::params(genre)
);
```

---

## Полезные функции PostgreSQL

### Строковые функции

```sql
-- Длина строки
SELECT LENGTH(title) FROM books;

-- Верхний регистр
SELECT UPPER(title) FROM books;

-- Нижний регистр
SELECT LOWER(title) FROM books;

-- Обрезка пробелов
SELECT TRIM(title) FROM books;
```

### Функции даты

```sql
-- Текущая дата
SELECT CURRENT_DATE;

-- Текущее время
SELECT CURRENT_TIMESTAMP;

-- Разница в днях
SELECT CURRENT_DATE - borrow_date FROM borrowings;
```

---

## Советы по написанию запросов

1. **Используйте алиасы** для читаемости:
   ```sql
   SELECT b.title, a.name AS author
   FROM books b
   JOIN authors a ON b.author_id = a.author_id;
   ```

2. **Группируйте логически** связанные условия:
   ```sql
   WHERE (author_id = 1 OR author_id = 2)
     AND total_copies > 3
   ```

3. **Используйте LIMIT** для больших таблиц:
   ```sql
   SELECT * FROM books LIMIT 10;
   ```

4. **Комментируйте сложные запросы:**
   ```sql
   -- Найти книги, которые выдавались более 2 раз
   SELECT b.title, COUNT(*) AS borrow_count
   FROM books b
   JOIN borrowings br ON b.book_id = br.book_id
   GROUP BY b.book_id, b.title
   HAVING COUNT(*) > 2;
   ```

---

*Справочник создан для лабораторной работы №6 по PostgreSQL*


