#include "Repositories.h"
#include <QDebug>


QList<Client> ClientRepository::getAll() {
    QList<Client> list;
    QSqlQuery query(
        "SELECT client_id, last_name, first_name, middle_name, birth_date, passport_series, passport_number, passport_issued_by, passport_issued_date, address, phone FROM Client ORDER BY client_id",
        DatabaseManager::instance().database());
    while (query.next()) {
        Client c;
        c.clientId = query.value(0).toInt();
        c.lastName = query.value(1).toString();
        c.firstName = query.value(2).toString();
        c.middleName = query.value(3).toString();
        c.birthDate = query.value(4).toDate();
        c.passportSeries = query.value(5).toString();
        c.passportNumber = query.value(6).toString();
        c.passportIssuedBy = query.value(7).toString();
        c.passportIssuedDate = query.value(8).toDate();
        c.address = query.value(9).toString();
        c.phone = query.value(10).toString();
        list.append(c);
    }
    return list;
}

std::optional<Client> ClientRepository::getById(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT client_id, last_name, first_name, middle_name, birth_date, passport_series, passport_number, passport_issued_by, passport_issued_date, address, phone FROM Client WHERE client_id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        Client c;
        c.clientId = query.value(0).toInt();
        c.lastName = query.value(1).toString();
        c.firstName = query.value(2).toString();
        c.middleName = query.value(3).toString();
        c.birthDate = query.value(4).toDate();
        c.passportSeries = query.value(5).toString();
        c.passportNumber = query.value(6).toString();
        c.passportIssuedBy = query.value(7).toString();
        c.passportIssuedDate = query.value(8).toDate();
        c.address = query.value(9).toString();
        c.phone = query.value(10).toString();
        return c;
    }
    return std::nullopt;
}

std::optional<Client> ClientRepository::getByUserId(int userId) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT c.client_id, c.last_name, c.first_name, c.middle_name, c.birth_date, c.passport_series, c.passport_number, c.passport_issued_by, c.passport_issued_date, c.address, c.phone "
        "FROM Client c "
        "JOIN \"User\" u ON c.client_id = u.client_id "
        "WHERE u.user_id = :userId");
    query.bindValue(":userId", userId);
    if (query.exec() && query.next()) {
        Client c;
        c.clientId = query.value(0).toInt();
        c.lastName = query.value(1).toString();
        c.firstName = query.value(2).toString();
        c.middleName = query.value(3).toString();
        c.birthDate = query.value(4).toDate();
        c.passportSeries = query.value(5).toString();
        c.passportNumber = query.value(6).toString();
        c.passportIssuedBy = query.value(7).toString();
        c.passportIssuedDate = query.value(8).toDate();
        c.address = query.value(9).toString();
        c.phone = query.value(10).toString();
        return c;
    }
    return std::nullopt;
}

bool ClientRepository::insert(Client &client) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "INSERT INTO Client (last_name, first_name, middle_name, birth_date, passport_series, passport_number, passport_issued_by, passport_issued_date, address, phone) "
        "VALUES (:last_name, :first_name, :middle_name, :birth_date, :passport_series, :passport_number, :passport_issued_by, :passport_issued_date, :address, :phone) "
        "RETURNING client_id");
    query.bindValue(":last_name", client.lastName);
    query.bindValue(":first_name", client.firstName);
    query.bindValue(":middle_name", client.middleName);
    query.bindValue(":birth_date", client.birthDate.isValid() ? QVariant(client.birthDate) : QVariant());
    query.bindValue(":passport_series", client.passportSeries);
    query.bindValue(":passport_number", client.passportNumber);
    query.bindValue(":passport_issued_by", client.passportIssuedBy);
    query.bindValue(":passport_issued_date",
                    client.passportIssuedDate.isValid() ? QVariant(client.passportIssuedDate) : QVariant());
    query.bindValue(":address", client.address);
    query.bindValue(":phone", client.phone);

    if (query.exec() && query.next()) {
        client.clientId = query.value(0).toInt();
        return true;
    }
    qWarning() << "Client insert failed:" << query.lastError().text();
    return false;
}

bool ClientRepository::update(const Client &client) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "UPDATE Client SET last_name = :last_name, first_name = :first_name, middle_name = :middle_name, birth_date = :birth_date, "
        "passport_series = :passport_series, passport_number = :passport_number, passport_issued_by = :passport_issued_by, "
        "passport_issued_date = :passport_issued_date, address = :address, phone = :phone WHERE client_id = :id");
    query.bindValue(":last_name", client.lastName);
    query.bindValue(":first_name", client.firstName);
    query.bindValue(":middle_name", client.middleName);
    query.bindValue(":birth_date", client.birthDate.isValid() ? QVariant(client.birthDate) : QVariant());
    query.bindValue(":passport_series", client.passportSeries);
    query.bindValue(":passport_number", client.passportNumber);
    query.bindValue(":passport_issued_by", client.passportIssuedBy);
    query.bindValue(":passport_issued_date",
                    client.passportIssuedDate.isValid() ? QVariant(client.passportIssuedDate) : QVariant());
    query.bindValue(":address", client.address);
    query.bindValue(":phone", client.phone);
    query.bindValue(":id", client.clientId);

    if (query.exec()) {
        return true;
    }
    qWarning() << "Client update failed:" << query.lastError().text();
    return false;
}

bool ClientRepository::remove(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM Client WHERE client_id = :id");
    query.bindValue(":id", id);
    return query.exec();
}


QList<Role> RoleRepository::getAll() {
    QList<Role> list;
    QSqlQuery query("SELECT role_id, role_name FROM Role ORDER BY role_id", DatabaseManager::instance().database());
    while (query.next()) {
        Role r;
        r.roleId = query.value(0).toInt();
        r.roleName = query.value(1).toString();
        list.append(r);
    }
    return list;
}

std::optional<Role> RoleRepository::getById(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT role_id, role_name FROM Role WHERE role_id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        Role r;
        r.roleId = query.value(0).toInt();
        r.roleName = query.value(1).toString();
        return r;
    }
    return std::nullopt;
}

std::optional<Role> RoleRepository::getByName(const QString &name) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT role_id, role_name FROM Role WHERE role_name = :name");
    query.bindValue(":name", name);
    if (query.exec() && query.next()) {
        Role r;
        r.roleId = query.value(0).toInt();
        r.roleName = query.value(1).toString();
        return r;
    }
    return std::nullopt;
}


QList<Item> ItemRepository::getAll() {
    QList<Item> list;
    QSqlQuery query("SELECT item_id, name, description, category, condition, status FROM Item ORDER BY item_id",
                    DatabaseManager::instance().database());
    while (query.next()) {
        Item i;
        i.itemId = query.value(0).toInt();
        i.name = query.value(1).toString();
        i.description = query.value(2).toString();
        i.category = query.value(3).toString();
        i.condition = query.value(4).toString();
        i.status = query.value(5).toString();
        list.append(i);
    }
    return list;
}

std::optional<Item> ItemRepository::getById(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT item_id, name, description, category, condition, status FROM Item WHERE item_id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        Item i;
        i.itemId = query.value(0).toInt();
        i.name = query.value(1).toString();
        i.description = query.value(2).toString();
        i.category = query.value(3).toString();
        i.condition = query.value(4).toString();
        i.status = query.value(5).toString();
        return i;
    }
    return std::nullopt;
}

bool ItemRepository::insert(Item &item) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "INSERT INTO Item (name, description, category, condition, status) VALUES (:name, :description, :category, :condition, :status) RETURNING item_id");
    query.bindValue(":name", item.name);
    query.bindValue(":description", item.description);
    query.bindValue(":category", item.category);
    query.bindValue(":condition", item.condition);
    query.bindValue(":status", item.status);

    if (query.exec() && query.next()) {
        item.itemId = query.value(0).toInt();
        return true;
    }
    qWarning() << "Item insert failed:" << query.lastError().text();
    return false;
}

bool ItemRepository::update(const Item &item) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "UPDATE Item SET name = :name, description = :description, category = :category, condition = :condition, status = :status WHERE item_id = :id");
    query.bindValue(":name", item.name);
    query.bindValue(":description", item.description);
    query.bindValue(":category", item.category);
    query.bindValue(":condition", item.condition);
    query.bindValue(":status", item.status);
    query.bindValue(":id", item.itemId);

    return query.exec();
}

bool ItemRepository::remove(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM Item WHERE item_id = :id");
    query.bindValue(":id", id);
    return query.exec();
}


QList<User> UserRepository::getAll() {
    QList<User> list;
    QSqlQuery query("SELECT user_id, client_id, login, password FROM \"User\" ORDER BY user_id",
                    DatabaseManager::instance().database());
    while (query.next()) {
        User u;
        u.userId = query.value(0).toInt();
        u.clientId = query.value(1).toInt();
        u.login = query.value(2).toString();
        u.password = query.value(3).toString();
        list.append(u);
    }
    return list;
}

std::optional<User> UserRepository::getById(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT user_id, client_id, login, password FROM \"User\" WHERE user_id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        User u;
        u.userId = query.value(0).toInt();
        u.clientId = query.value(1).toInt();
        u.login = query.value(2).toString();
        u.password = query.value(3).toString();
        return u;
    }
    return std::nullopt;
}

std::optional<User> UserRepository::getByLogin(const QString &login) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT user_id, client_id, login, password FROM \"User\" WHERE login = :login");
    query.bindValue(":login", login);
    if (query.exec() && query.next()) {
        User u;
        u.userId = query.value(0).toInt();
        u.clientId = query.value(1).toInt();
        u.login = query.value(2).toString();
        u.password = query.value(3).toString();
        return u;
    }
    return std::nullopt;
}

bool UserRepository::insert(User &user) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "INSERT INTO \"User\" (client_id, login, password) VALUES (:client_id, :login, :password) RETURNING user_id");
    query.bindValue(":client_id", user.clientId);
    query.bindValue(":login", user.login);
    query.bindValue(":password", user.password);

    if (query.exec() && query.next()) {
        user.userId = query.value(0).toInt();
        return true;
    }
    qWarning() << "User insert failed:" << query.lastError().text();
    return false;
}

bool UserRepository::update(const User &user) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "UPDATE \"User\" SET client_id = :client_id, login = :login, password = :password WHERE user_id = :id");
    query.bindValue(":client_id", user.clientId);
    query.bindValue(":login", user.login);
    query.bindValue(":password", user.password);
    query.bindValue(":id", user.userId);

    return query.exec();
}

bool UserRepository::remove(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM \"User\" WHERE user_id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

QString UserRepository::getUserRoleName(int userId) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT r.role_name FROM Role r JOIN User_Role ur ON r.role_id = ur.role_id WHERE ur.user_id = :userId");
    query.bindValue(":userId", userId);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return "";
}

bool UserRepository::setUserRole(int userId, int roleId) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM User_Role WHERE user_id = :userId");
    query.bindValue(":userId", userId);
    if (!query.exec()) return false;

    query.prepare("INSERT INTO User_Role (user_id, role_id) VALUES (:userId, :roleId)");
    query.bindValue(":userId", userId);
    query.bindValue(":roleId", roleId);
    return query.exec();
}


QList<PledgeContract> PledgeContractRepository::getAll() {
    QList<PledgeContract> list;
    QSqlQuery query(
        "SELECT contract_id, client_id, contract_number, date_concluded, commission, return_date, contract_status FROM Pledge_Contract ORDER BY contract_id",
        DatabaseManager::instance().database());
    while (query.next()) {
        PledgeContract pc;
        pc.contractId = query.value(0).toInt();
        pc.clientId = query.value(1).toInt();
        pc.contractNumber = query.value(2).toString();
        pc.dateConcluded = query.value(3).toDate();
        pc.commission = query.value(4).toDouble();
        pc.returnDate = query.value(5).toDate();
        pc.contractStatus = query.value(6).toString();
        list.append(pc);
    }
    return list;
}

QList<PledgeContract> PledgeContractRepository::getByClientId(int clientId) {
    QList<PledgeContract> list;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT contract_id, client_id, contract_number, date_concluded, commission, return_date, contract_status FROM Pledge_Contract WHERE client_id = :clientId ORDER BY contract_id");
    query.bindValue(":clientId", clientId);
    if (query.exec()) {
        while (query.next()) {
            PledgeContract pc;
            pc.contractId = query.value(0).toInt();
            pc.clientId = query.value(1).toInt();
            pc.contractNumber = query.value(2).toString();
            pc.dateConcluded = query.value(3).toDate();
            pc.commission = query.value(4).toDouble();
            pc.returnDate = query.value(5).toDate();
            pc.contractStatus = query.value(6).toString();
            list.append(pc);
        }
    }
    return list;
}

std::optional<PledgeContract> PledgeContractRepository::getById(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT contract_id, client_id, contract_number, date_concluded, commission, return_date, contract_status FROM Pledge_Contract WHERE contract_id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        PledgeContract pc;
        pc.contractId = query.value(0).toInt();
        pc.clientId = query.value(1).toInt();
        pc.contractNumber = query.value(2).toString();
        pc.dateConcluded = query.value(3).toDate();
        pc.commission = query.value(4).toDouble();
        pc.returnDate = query.value(5).toDate();
        pc.contractStatus = query.value(6).toString();
        return pc;
    }
    return std::nullopt;
}

bool PledgeContractRepository::insert(PledgeContract &contract) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "INSERT INTO Pledge_Contract (client_id, contract_number, date_concluded, commission, return_date, contract_status) "
        "VALUES (:client_id, :contract_number, :date_concluded, :commission, :return_date, :contract_status) RETURNING contract_id");
    query.bindValue(":client_id", contract.clientId);
    query.bindValue(":contract_number", contract.contractNumber);
    query.bindValue(":date_concluded", contract.dateConcluded);
    query.bindValue(":commission", contract.commission);
    query.bindValue(":return_date", contract.returnDate.isValid() ? QVariant(contract.returnDate) : QVariant());
    query.bindValue(":contract_status", contract.contractStatus);

    if (query.exec() && query.next()) {
        contract.contractId = query.value(0).toInt();
        return true;
    }
    qWarning() << "PledgeContract insert failed:" << query.lastError().text();
    return false;
}

bool PledgeContractRepository::update(const PledgeContract &contract) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "UPDATE Pledge_Contract SET client_id = :client_id, contract_number = :contract_number, date_concluded = :date_concluded, "
        "commission = :commission, return_date = :return_date, contract_status = :contract_status WHERE contract_id = :id");
    query.bindValue(":client_id", contract.clientId);
    query.bindValue(":contract_number", contract.contractNumber);
    query.bindValue(":date_concluded", contract.dateConcluded);
    query.bindValue(":commission", contract.commission);
    query.bindValue(":return_date", contract.returnDate.isValid() ? QVariant(contract.returnDate) : QVariant());
    query.bindValue(":contract_status", contract.contractStatus);
    query.bindValue(":id", contract.contractId);

    return query.exec();
}

bool PledgeContractRepository::remove(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM Pledge_Contract WHERE contract_id = :id");
    query.bindValue(":id", id);
    return query.exec();
}


QList<ContractPosition> ContractPositionRepository::getAll() {
    QList<ContractPosition> list;
    QSqlQuery query(
        "SELECT position_id, contract_id, item_id, estimated_value, loan_amount FROM Contract_Position ORDER BY position_id",
        DatabaseManager::instance().database());
    while (query.next()) {
        ContractPosition cp;
        cp.positionId = query.value(0).toInt();
        cp.contractId = query.value(1).toInt();
        cp.itemId = query.value(2).toInt();
        cp.estimatedValue = query.value(3).toDouble();
        cp.loanAmount = query.value(4).toDouble();
        list.append(cp);
    }
    return list;
}

QList<ContractPosition> ContractPositionRepository::getByContractId(int contractId) {
    QList<ContractPosition> list;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT position_id, contract_id, item_id, estimated_value, loan_amount FROM Contract_Position WHERE contract_id = :contractId ORDER BY position_id");
    query.bindValue(":contractId", contractId);
    if (query.exec()) {
        while (query.next()) {
            ContractPosition cp;
            cp.positionId = query.value(0).toInt();
            cp.contractId = query.value(1).toInt();
            cp.itemId = query.value(2).toInt();
            cp.estimatedValue = query.value(3).toDouble();
            cp.loanAmount = query.value(4).toDouble();
            list.append(cp);
        }
    }
    return list;
}

std::optional<ContractPosition> ContractPositionRepository::getById(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT position_id, contract_id, item_id, estimated_value, loan_amount FROM Contract_Position WHERE position_id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        ContractPosition cp;
        cp.positionId = query.value(0).toInt();
        cp.contractId = query.value(1).toInt();
        cp.itemId = query.value(2).toInt();
        cp.estimatedValue = query.value(3).toDouble();
        cp.loanAmount = query.value(4).toDouble();
        return cp;
    }
    return std::nullopt;
}

bool ContractPositionRepository::insert(ContractPosition &position) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("INSERT INTO Contract_Position (contract_id, item_id, estimated_value, loan_amount) "
        "VALUES (:contract_id, :item_id, :estimated_value, :loan_amount) RETURNING position_id");
    query.bindValue(":contract_id", position.contractId);
    query.bindValue(":item_id", position.itemId);
    query.bindValue(":estimated_value", position.estimatedValue);
    query.bindValue(":loan_amount", position.loanAmount);

    if (query.exec() && query.next()) {
        position.positionId = query.value(0).toInt();
        return true;
    }
    qWarning() << "ContractPosition insert failed:" << query.lastError().text();
    return false;
}

bool ContractPositionRepository::update(const ContractPosition &position) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE Contract_Position SET contract_id = :contract_id, item_id = :item_id, "
        "estimated_value = :estimated_value, loan_amount = :loan_amount WHERE position_id = :id");
    query.bindValue(":contract_id", position.contractId);
    query.bindValue(":item_id", position.itemId);
    query.bindValue(":estimated_value", position.estimatedValue);
    query.bindValue(":loan_amount", position.loanAmount);
    query.bindValue(":id", position.positionId);

    return query.exec();
}

bool ContractPositionRepository::remove(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM Contract_Position WHERE position_id = :id");
    query.bindValue(":id", id);
    return query.exec();
}


QList<Repayment> RepaymentRepository::getAll() {
    QList<Repayment> list;
    QSqlQuery query(
        "SELECT repayment_id, position_id, repayment_date, repayment_amount, payment_method FROM Repayment ORDER BY repayment_id",
        DatabaseManager::instance().database());
    while (query.next()) {
        Repayment r;
        r.repaymentId = query.value(0).toInt();
        r.positionId = query.value(1).toInt();
        r.repaymentDate = query.value(2).toDate();
        r.repaymentAmount = query.value(3).toDouble();
        r.paymentMethod = query.value(4).toString();
        list.append(r);
    }
    return list;
}

QList<Repayment> RepaymentRepository::getByPositionId(int positionId) {
    QList<Repayment> list;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT repayment_id, position_id, repayment_date, repayment_amount, payment_method FROM Repayment WHERE position_id = :positionId ORDER BY repayment_id");
    query.bindValue(":positionId", positionId);
    if (query.exec()) {
        while (query.next()) {
            Repayment r;
            r.repaymentId = query.value(0).toInt();
            r.positionId = query.value(1).toInt();
            r.repaymentDate = query.value(2).toDate();
            r.repaymentAmount = query.value(3).toDouble();
            r.paymentMethod = query.value(4).toString();
            list.append(r);
        }
    }
    return list;
}

std::optional<Repayment> RepaymentRepository::getById(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT repayment_id, position_id, repayment_date, repayment_amount, payment_method FROM Repayment WHERE repayment_id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        Repayment r;
        r.repaymentId = query.value(0).toInt();
        r.positionId = query.value(1).toInt();
        r.repaymentDate = query.value(2).toDate();
        r.repaymentAmount = query.value(3).toDouble();
        r.paymentMethod = query.value(4).toString();
        return r;
    }
    return std::nullopt;
}

bool RepaymentRepository::insert(Repayment &repayment) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("INSERT INTO Repayment (position_id, repayment_date, repayment_amount, payment_method) "
        "VALUES (:position_id, :repayment_date, :repayment_amount, :payment_method) RETURNING repayment_id");
    query.bindValue(":position_id", repayment.positionId);
    query.bindValue(":repayment_date", repayment.repaymentDate);
    query.bindValue(":repayment_amount", repayment.repaymentAmount);
    query.bindValue(":payment_method", repayment.paymentMethod);

    if (query.exec() && query.next()) {
        repayment.repaymentId = query.value(0).toInt();
        return true;
    }
    qWarning() << "Repayment insert failed:" << query.lastError().text();
    return false;
}

bool RepaymentRepository::update(const Repayment &repayment) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE Repayment SET position_id = :position_id, repayment_date = :repayment_date, "
        "repayment_amount = :repayment_amount, payment_method = :payment_method WHERE repayment_id = :id");
    query.bindValue(":position_id", repayment.positionId);
    query.bindValue(":repayment_date", repayment.repaymentDate);
    query.bindValue(":repayment_amount", repayment.repaymentAmount);
    query.bindValue(":payment_method", repayment.paymentMethod);
    query.bindValue(":id", repayment.repaymentId);

    return query.exec();
}

bool RepaymentRepository::remove(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM Repayment WHERE repayment_id = :id");
    query.bindValue(":id", id);
    return query.exec();
}


QList<Revaluation> RevaluationRepository::getAll() {
    QList<Revaluation> list;
    QSqlQuery query(
        "SELECT revaluation_id, item_id, change_date, new_price, reason FROM Revaluation ORDER BY revaluation_id",
        DatabaseManager::instance().database());
    while (query.next()) {
        Revaluation r;
        r.revaluationId = query.value(0).toInt();
        r.itemId = query.value(1).toInt();
        r.changeDate = query.value(2).toDate();
        r.newPrice = query.value(3).toDouble();
        r.reason = query.value(4).toString();
        list.append(r);
    }
    return list;
}

QList<Revaluation> RevaluationRepository::getByItemId(int itemId) {
    QList<Revaluation> list;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT revaluation_id, item_id, change_date, new_price, reason FROM Revaluation WHERE item_id = :itemId ORDER BY change_date DESC");
    query.bindValue(":itemId", itemId);
    if (query.exec()) {
        while (query.next()) {
            Revaluation r;
            r.revaluationId = query.value(0).toInt();
            r.itemId = query.value(1).toInt();
            r.changeDate = query.value(2).toDate();
            r.newPrice = query.value(3).toDouble();
            r.reason = query.value(4).toString();
            list.append(r);
        }
    }
    return list;
}

std::optional<Revaluation> RevaluationRepository::getById(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT revaluation_id, item_id, change_date, new_price, reason FROM Revaluation WHERE revaluation_id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        Revaluation r;
        r.revaluationId = query.value(0).toInt();
        r.itemId = query.value(1).toInt();
        r.changeDate = query.value(2).toDate();
        r.newPrice = query.value(3).toDouble();
        r.reason = query.value(4).toString();
        return r;
    }
    return std::nullopt;
}

bool RevaluationRepository::insert(Revaluation &revaluation) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("INSERT INTO Revaluation (item_id, change_date, new_price, reason) "
        "VALUES (:item_id, :change_date, :new_price, :reason) RETURNING revaluation_id");
    query.bindValue(":item_id", revaluation.itemId);
    query.bindValue(":change_date", revaluation.changeDate);
    query.bindValue(":new_price", revaluation.newPrice);
    query.bindValue(":reason", revaluation.reason);

    if (query.exec() && query.next()) {
        revaluation.revaluationId = query.value(0).toInt();
        return true;
    }
    qWarning() << "Revaluation insert failed:" << query.lastError().text();
    return false;
}

bool RevaluationRepository::update(const Revaluation &revaluation) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE Revaluation SET item_id = :item_id, change_date = :change_date, "
        "new_price = :new_price, reason = :reason WHERE revaluation_id = :id");
    query.bindValue(":item_id", revaluation.itemId);
    query.bindValue(":change_date", revaluation.changeDate);
    query.bindValue(":new_price", revaluation.newPrice);
    query.bindValue(":reason", revaluation.reason);
    query.bindValue(":id", revaluation.revaluationId);

    return query.exec();
}

bool RevaluationRepository::remove(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM Revaluation WHERE revaluation_id = :id");
    query.bindValue(":id", id);
    return query.exec();
}
