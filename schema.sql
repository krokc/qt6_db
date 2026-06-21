-- 1. Таблица Client (Клиент)
CREATE TABLE Client (
    client_id SERIAL PRIMARY KEY,
    last_name VARCHAR(100) NOT NULL,
    first_name VARCHAR(100) NOT NULL,
    middle_name VARCHAR(100),
    birth_date DATE,
    passport_series VARCHAR(10) NOT NULL,
    passport_number VARCHAR(20) NOT NULL,
    passport_issued_by VARCHAR(255),
    passport_issued_date DATE,
    address TEXT,
    phone VARCHAR(20) NOT NULL,
    
    CONSTRAINT uq_passport UNIQUE (passport_series, passport_number),
    CONSTRAINT uq_phone UNIQUE (phone)
);

-- 2. Таблица Role (Роль)
CREATE TABLE Role (
    role_id SERIAL PRIMARY KEY,
    role_name VARCHAR(50) NOT NULL,
    
    CONSTRAINT uq_role_name UNIQUE (role_name)
);

-- 3. Таблица Item (Товар)
CREATE TABLE Item (
    item_id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    description TEXT,
    category VARCHAR(100),
    condition VARCHAR(100),
    status VARCHAR(50)
);

-- 4. Таблица User (Пользователь)
CREATE TABLE "User" (
    user_id SERIAL PRIMARY KEY,
    client_id INT NOT NULL,
    login VARCHAR(50) NOT NULL,
    password VARCHAR(255) NOT NULL,
    
    CONSTRAINT uq_login UNIQUE (login),
    CONSTRAINT uq_user_client UNIQUE (client_id),
    FOREIGN KEY (client_id) REFERENCES Client(client_id) ON DELETE CASCADE
);

-- 5. Таблица User_Role (Пользователь_Роль)
CREATE TABLE User_Role (
    user_id INT,
    role_id INT,
    
    PRIMARY KEY (user_id, role_id),
    FOREIGN KEY (user_id) REFERENCES "User"(user_id) ON DELETE CASCADE,
    FOREIGN KEY (role_id) REFERENCES Role(role_id) ON DELETE CASCADE
);

-- 6. Таблица Pledge_Contract (Договор_залога)
CREATE TABLE Pledge_Contract (
    contract_id SERIAL PRIMARY KEY,
    client_id INT NOT NULL,
    contract_number VARCHAR(50) NOT NULL,
    date_concluded DATE NOT NULL,
    commission DECIMAL(10, 2),
    return_date DATE,
    contract_status VARCHAR(50),
    
    CONSTRAINT uq_contract_number UNIQUE (contract_number),
    FOREIGN KEY (client_id) REFERENCES Client(client_id) ON DELETE CASCADE
);

-- 7. Таблица Contract_Position (Позиция_договора)
CREATE TABLE Contract_Position (
    position_id SERIAL PRIMARY KEY,
    contract_id INT NOT NULL,
    item_id INT NOT NULL,
    estimated_value DECIMAL(10, 2) NOT NULL,
    loan_amount DECIMAL(10, 2) NOT NULL,
    
    CONSTRAINT uq_contract_item UNIQUE (contract_id, item_id),
    FOREIGN KEY (contract_id) REFERENCES Pledge_Contract(contract_id) ON DELETE CASCADE,
    FOREIGN KEY (item_id) REFERENCES Item(item_id) ON DELETE CASCADE
);

-- 8. Таблица Repayment (Возврат_средств)
CREATE TABLE Repayment (
    repayment_id SERIAL PRIMARY KEY,
    position_id INT NOT NULL,
    repayment_date DATE NOT NULL,
    repayment_amount DECIMAL(10, 2) NOT NULL,
    payment_method VARCHAR(50),
    
    CONSTRAINT uq_return_position UNIQUE (position_id),
    FOREIGN KEY (position_id) REFERENCES Contract_Position(position_id) ON DELETE CASCADE
);

-- 9. Таблица Revaluation (Переоценка)
CREATE TABLE Revaluation (
    revaluation_id SERIAL PRIMARY KEY,
    item_id INT NOT NULL,
    change_date DATE NOT NULL,
    new_price DECIMAL(10, 2) NOT NULL,
    reason TEXT,
    
    CONSTRAINT uq_revaluation_item_date UNIQUE (item_id, change_date),
    FOREIGN KEY (item_id) REFERENCES Item(item_id) ON DELETE CASCADE
);

-- Наполнение ролей
INSERT INTO Role (role_name) VALUES ('admin');
INSERT INTO Role (role_name) VALUES ('client');

-- Создание дефолтного администратора
-- 1. Сначала клиент
INSERT INTO Client (last_name, first_name, middle_name, passport_series, passport_number, phone)
VALUES ('Администратор', 'Главный', '', '0000', '000000', '+70000000000');

-- 2. Затем пользователь (admin / Admin@123)
INSERT INTO "User" (client_id, login, password)
VALUES (1, 'admin', 'Admin@123');

-- 3. Связываем роль администратора
INSERT INTO User_Role (user_id, role_id)
VALUES (1, 1);
