#pragma once
#include <QSqlQuery>
#include <optional>
#include "Entities.h"
#include "DatabaseManager.h"

class ClientRepository {
public:
    static QList<Client> getAll();
    static std::optional<Client> getById(int id);
    static std::optional<Client> getByUserId(int userId);
    static bool insert(Client& client);
    static bool update(const Client& client);
    static bool remove(int id);
};

class RoleRepository {
public:
    static QList<Role> getAll();
    static std::optional<Role> getById(int id);
    static std::optional<Role> getByName(const QString& name);
};

class ItemRepository {
public:
    static QList<Item> getAll();
    static std::optional<Item> getById(int id);
    static bool insert(Item& item);
    static bool update(const Item& item);
    static bool remove(int id);
};

class UserRepository {
public:
    static QList<User> getAll();
    static std::optional<User> getById(int id);
    static std::optional<User> getByLogin(const QString& login);
    static bool insert(User& user);
    static bool update(const User& user);
    static bool remove(int id);
    
    static QString getUserRoleName(int userId);
    static bool setUserRole(int userId, int roleId);
};

class PledgeContractRepository {
public:
    static QList<PledgeContract> getAll();
    static QList<PledgeContract> getByClientId(int clientId);
    static std::optional<PledgeContract> getById(int id);
    static bool insert(PledgeContract& contract);
    static bool update(const PledgeContract& contract);
    static bool remove(int id);
};

class ContractPositionRepository {
public:
    static QList<ContractPosition> getAll();
    static QList<ContractPosition> getByContractId(int contractId);
    static std::optional<ContractPosition> getById(int id);
    static bool insert(ContractPosition& position);
    static bool update(const ContractPosition& position);
    static bool remove(int id);
};

class RepaymentRepository {
public:
    static QList<Repayment> getAll();
    static QList<Repayment> getByPositionId(int positionId);
    static std::optional<Repayment> getById(int id);
    static bool insert(Repayment& repayment);
    static bool update(const Repayment& repayment);
    static bool remove(int id);
};

class RevaluationRepository {
public:
    static QList<Revaluation> getAll();
    static QList<Revaluation> getByItemId(int itemId);
    static std::optional<Revaluation> getById(int id);
    static bool insert(Revaluation& revaluation);
    static bool update(const Revaluation& revaluation);
    static bool remove(int id);
};
