-- Схема БД для системы туристического агентства
-- Тема: туристическое агентство (клиенты, туры, отели, бронирования, платежи)

DROP TABLE IF EXISTS payments CASCADE;
DROP TABLE IF EXISTS bookings CASCADE;
DROP TABLE IF EXISTS tours CASCADE;
DROP TABLE IF EXISTS hotels CASCADE;
DROP TABLE IF EXISTS clients CASCADE;

-- 1. Клиенты
CREATE TABLE clients (
    client_id   SERIAL PRIMARY KEY,
    full_name   TEXT NOT NULL,
    email       TEXT UNIQUE,
    phone       TEXT,
    passport    TEXT
);

-- 2. Отели
CREATE TABLE hotels (
    hotel_id    SERIAL PRIMARY KEY,
    name        TEXT NOT NULL,
    country     TEXT NOT NULL,
    city        TEXT NOT NULL,
    stars       INT  CHECK (stars >= 1 AND stars <= 5),
    address     TEXT
);

-- 3. Туры
CREATE TABLE tours (
    tour_id     SERIAL PRIMARY KEY,
    title       TEXT NOT NULL,
    hotel_id    INT  NOT NULL REFERENCES hotels(hotel_id) ON DELETE CASCADE,
    duration    INT  NOT NULL CHECK (duration > 0),
    price       DECIMAL(10, 2) NOT NULL CHECK (price >= 0),
    start_date  DATE,
    description TEXT
);

-- 4. Бронирования
CREATE TABLE bookings (
    booking_id  SERIAL PRIMARY KEY,
    client_id   INT  NOT NULL REFERENCES clients(client_id) ON DELETE CASCADE,
    tour_id     INT  NOT NULL REFERENCES tours(tour_id)     ON DELETE CASCADE,
    booking_date DATE NOT NULL,
    status      TEXT NOT NULL DEFAULT 'pending' CHECK (status IN ('pending', 'confirmed', 'cancelled', 'completed'))
);

-- 5. Платежи
CREATE TABLE payments (
    payment_id  SERIAL PRIMARY KEY,
    booking_id  INT  NOT NULL REFERENCES bookings(booking_id) ON DELETE CASCADE,
    amount      DECIMAL(10, 2) NOT NULL CHECK (amount >= 0),
    payment_date DATE NOT NULL,
    payment_method TEXT CHECK (payment_method IN ('cash', 'card', 'bank_transfer', 'online'))
);

-- Начальные данные

INSERT INTO clients (full_name, email, phone, passport) VALUES
('Иван Иванов', 'ivan@example.com', '+7-900-123-45-67', '1234 567890'),
('Пётр Петров', 'petr@example.com', '+7-900-234-56-78', '2345 678901'),
('Мария Смирнова', 'maria@example.com', '+7-900-345-67-89', '3456 789012'),
('Анна Козлова', 'anna@example.com', '+7-900-456-78-90', '4567 890123');

INSERT INTO hotels (name, country, city, stars, address) VALUES
('Гранд Отель Европа', 'Россия', 'Санкт-Петербург', 5, 'Михайловская ул., 1/7'),
('Морской бриз', 'Турция', 'Анталия', 4, 'Konyaalti Beach, 07070'),
('Пальмовый рай', 'ОАЭ', 'Дубай', 5, 'Palm Jumeirah, Jumeirah'),
('Альпийская долина', 'Австрия', 'Инсбрук', 4, 'Höhenstraße 1, 6020'),
('Тропическая лагуна', 'Таиланд', 'Пхукет', 5, 'Patong Beach Road');

INSERT INTO tours (title, hotel_id, duration, price, start_date, description) VALUES
('Экскурсионный тур по Санкт-Петербургу', 1, 5, 45000.00, '2025-06-01', 'Обзорные экскурсии по историческому центру'),
('Отдых на пляжах Анталии', 2, 7, 75000.00, '2025-07-15', 'Пляжный отдых с питанием all inclusive'),
('Роскошный отдых в Дубае', 3, 10, 180000.00, '2025-08-01', 'Премиум отдых с экскурсиями'),
('Горнолыжный курорт в Альпах', 4, 6, 95000.00, '2025-12-20', 'Катание на лыжах в Австрийских Альпах'),
('Тропический рай на Пхукете', 5, 14, 120000.00, '2025-09-10', 'Пляжный отдых на острове');

INSERT INTO bookings (client_id, tour_id, booking_date, status) VALUES
(1, 1, '2025-04-10', 'confirmed'),
(2, 2, '2025-05-15', 'confirmed'),
(3, 3, '2025-06-01', 'pending'),
(1, 4, '2025-05-20', 'confirmed'),
(4, 5, '2025-07-05', 'confirmed'),
(2, 3, '2025-06-10', 'cancelled');

INSERT INTO payments (booking_id, amount, payment_date, payment_method) VALUES
(1, 45000.00, '2025-04-10', 'card'),
(2, 75000.00, '2025-05-15', 'bank_transfer'),
(4, 95000.00, '2025-05-20', 'card'),
(5, 120000.00, '2025-07-05', 'online'),
(2, 25000.00, '2025-05-16', 'cash');
