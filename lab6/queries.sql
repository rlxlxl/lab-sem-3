-- Примеры запросов для лабораторной работы

-- 1. Список всех книг с авторами и жанрами (INNER JOIN)
SELECT b.title, a.name AS author, g.name AS genre
FROM books b
JOIN authors a ON b.author_id = a.author_id
JOIN genres g  ON b.genre_id  = g.genre_id;

-- 2. Все читатели и их текущие невозвращённые книги (LEFT JOIN + WHERE)
SELECT r.full_name, b.title, br.borrow_date
FROM readers r
LEFT JOIN borrowings br ON r.reader_id = br.reader_id AND br.return_date IS NULL
LEFT JOIN books b       ON br.book_id   = b.book_id
WHERE br.borrowing_id IS NOT NULL;

-- 3. Количество выданных книг по каждому читателю (GROUP BY + агрегатная функция COUNT)
SELECT r.full_name,
       COUNT(br.borrowing_id) AS total_borrowed
FROM readers r
LEFT JOIN borrowings br ON r.reader_id = br.reader_id
GROUP BY r.reader_id, r.full_name
ORDER BY total_borrowed DESC;

-- 4. Самые популярные книги (количество выдач) с использованием HAVING
SELECT b.title,
       COUNT(br.borrowing_id) AS borrow_count
FROM books b
JOIN borrowings br ON b.book_id = br.book_id
GROUP BY b.book_id, b.title
HAVING COUNT(br.borrowing_id) >= 2
ORDER BY borrow_count DESC;

-- 5. INNER JOIN: читатели и книги, которые они брали
SELECT r.full_name, b.title, br.borrow_date, br.return_date
FROM readers r
INNER JOIN borrowings br ON r.reader_id = br.reader_id
INNER JOIN books b       ON br.book_id   = b.book_id
ORDER BY br.borrow_date;

-- 6. RIGHT JOIN: все выдачи и связанные читатели
SELECT br.borrowing_id, r.full_name, br.borrow_date
FROM borrowings br
RIGHT JOIN readers r ON br.reader_id = r.reader_id
ORDER BY r.full_name;

-- 7. FULL JOIN: все книги и все выдачи (покажет также книги, которые никто не брал)
SELECT b.title, br.borrow_date, br.return_date
FROM books b
FULL JOIN borrowings br ON b.book_id = br.book_id
ORDER BY b.title;

-- 8. Подзапрос: книги, которые никогда не выдавались
SELECT title
FROM books
WHERE book_id NOT IN (
    SELECT DISTINCT book_id FROM borrowings
);

-- 9. UNION: читатели, у которых есть e-mail, и фиктивный пользователь без e-mail
SELECT full_name, email
FROM readers
WHERE email IS NOT NULL
UNION
SELECT 'Без имени' AS full_name, NULL::TEXT AS email;

-- 10. Поиск возможных дубликатов читателей по совпадающим именам
SELECT r1.reader_id, r1.full_name, r1.email
FROM readers r1
JOIN readers r2
  ON r1.full_name = r2.full_name
 AND r1.reader_id <> r2.reader_id
ORDER BY r1.full_name;


