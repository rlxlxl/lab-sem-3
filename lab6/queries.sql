-- Примеры запросов для туристического агентства

-- 1. Список всех туров с отелями (INNER JOIN)
SELECT t.title, h.name AS hotel, h.city, h.country, t.price, t.duration
FROM tours t
JOIN hotels h ON t.hotel_id = h.hotel_id;

-- 2. Все клиенты и их текущие бронирования (LEFT JOIN + WHERE)
SELECT c.full_name, t.title, b.booking_date, b.status
FROM clients c
LEFT JOIN bookings b ON c.client_id = b.client_id AND b.status = 'confirmed'
LEFT JOIN tours t ON b.tour_id = t.tour_id
WHERE b.booking_id IS NOT NULL;

-- 3. Количество бронирований по каждому клиенту (GROUP BY + агрегатная функция COUNT)
SELECT c.full_name,
       COUNT(b.booking_id) AS total_bookings
FROM clients c
LEFT JOIN bookings b ON c.client_id = b.client_id
GROUP BY c.client_id, c.full_name
ORDER BY total_bookings DESC;

-- 4. Самые популярные туры (количество бронирований) с использованием HAVING
SELECT t.title,
       COUNT(b.booking_id) AS booking_count
FROM tours t
JOIN bookings b ON t.tour_id = b.tour_id
GROUP BY t.tour_id, t.title
HAVING COUNT(b.booking_id) >= 1
ORDER BY booking_count DESC;

-- 5. INNER JOIN: клиенты и туры, которые они забронировали
SELECT c.full_name, t.title, h.city, b.booking_date, b.status
FROM clients c
INNER JOIN bookings b ON c.client_id = b.client_id
INNER JOIN tours t ON b.tour_id = t.tour_id
INNER JOIN hotels h ON t.hotel_id = h.hotel_id
ORDER BY b.booking_date;

-- 6. RIGHT JOIN: все бронирования и связанные клиенты
SELECT b.booking_id, c.full_name, t.title, b.booking_date
FROM bookings b
RIGHT JOIN clients c ON b.client_id = c.client_id
LEFT JOIN tours t ON b.tour_id = t.tour_id
ORDER BY c.full_name;

-- 7. FULL JOIN: все туры и все бронирования (покажет также туры без бронирований)
SELECT t.title, b.booking_date, b.status
FROM tours t
FULL JOIN bookings b ON t.tour_id = b.tour_id
ORDER BY t.title;

-- 8. Подзапрос: туры, которые никогда не бронировались
SELECT title
FROM tours
WHERE tour_id NOT IN (
    SELECT DISTINCT tour_id FROM bookings WHERE status != 'cancelled'
);

-- 9. UNION: клиенты, у которых есть email, и фиктивный клиент без email
SELECT full_name, email
FROM clients
WHERE email IS NOT NULL
UNION
SELECT 'Незарегистрированный клиент' AS full_name, NULL::TEXT AS email;

-- 10. Поиск возможных дубликатов клиентов по совпадающим именам
SELECT c1.client_id, c1.full_name, c1.email
FROM clients c1
JOIN clients c2
  ON c1.full_name = c2.full_name
 AND c1.client_id <> c2.client_id
ORDER BY c1.full_name;

-- 11. Общая сумма платежей по каждому бронированию
SELECT b.booking_id, c.full_name, t.title, 
       COALESCE(SUM(p.amount), 0) AS total_paid
FROM bookings b
JOIN clients c ON b.client_id = c.client_id
JOIN tours t ON b.tour_id = t.tour_id
LEFT JOIN payments p ON b.booking_id = p.booking_id
GROUP BY b.booking_id, c.full_name, t.title
ORDER BY total_paid DESC;

-- 12. Отели по странам с количеством туров
SELECT h.country, h.city, COUNT(t.tour_id) AS tours_count
FROM hotels h
LEFT JOIN tours t ON h.hotel_id = t.hotel_id
GROUP BY h.country, h.city
ORDER BY tours_count DESC, h.country;

-- 13. Клиенты с незавершенными платежами (сумма платежей меньше стоимости тура)
SELECT c.full_name, t.title, t.price AS tour_price,
       COALESCE(SUM(p.amount), 0) AS paid_amount,
       (t.price - COALESCE(SUM(p.amount), 0)) AS remaining_amount
FROM bookings b
JOIN clients c ON b.client_id = c.client_id
JOIN tours t ON b.tour_id = t.tour_id
LEFT JOIN payments p ON b.booking_id = p.booking_id
WHERE b.status IN ('confirmed', 'pending')
GROUP BY c.full_name, t.title, t.price, b.booking_id
HAVING COALESCE(SUM(p.amount), 0) < t.price;
