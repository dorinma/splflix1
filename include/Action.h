#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>

class Session;

enum ActionStatus{
	PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
	BaseAction();
	BaseAction(const BaseAction &other); //copy constructor
	BaseAction& operator=(const BaseAction& other); //copy assignment operator
    virtual ~BaseAction(); //destructor
    virtual BaseAction* dupAction() = 0;
	ActionStatus getStatus() const;
	virtual void act(Session& sess)=0;
	virtual std::string toString() const=0;
protected:
	void complete();
	void error(const std::string& errorMsg);
	std::string getErrorMsg() const;
private:
	std::string errorMsg;
	ActionStatus status = PENDING;
};

class CreateUser  : public BaseAction {
public:
    CreateUser();
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual CreateUser* dupAction();
    virtual ~CreateUser();
    CreateUser(const CreateUser& other);
};

class ChangeActiveUser : public BaseAction {
public:
    ChangeActiveUser();
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual ChangeActiveUser* dupAction();
    virtual ~ChangeActiveUser();
    ChangeActiveUser(const ChangeActiveUser& other);
};

class DeleteUser : public BaseAction {
public:
    DeleteUser();
	virtual void act(Session & sess);
	virtual std::string toString() const;
    virtual DeleteUser* dupAction();
    virtual ~DeleteUser();
    DeleteUser(const DeleteUser& other);
};


class DuplicateUser : public BaseAction {
public:
    DuplicateUser();
	virtual void act(Session & sess);
	virtual std::string toString() const;
    virtual DuplicateUser* dupAction();
    virtual ~DuplicateUser();
    DuplicateUser(const DuplicateUser& other);
};

class PrintContentList : public BaseAction {
public:
    PrintContentList();
	virtual void act (Session& sess);
	virtual std::string toString() const;
    virtual PrintContentList* dupAction();
    virtual ~PrintContentList();
    PrintContentList(const PrintContentList& other);
};

class PrintWatchHistory : public BaseAction {
public:
    PrintWatchHistory();
	virtual void act (Session& sess);
	virtual std::string toString() const;
    virtual PrintWatchHistory* dupAction();
    virtual ~PrintWatchHistory();
    PrintWatchHistory(const PrintWatchHistory& other);
};


class Watch : public BaseAction {
public:
    Watch();
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual Watch* dupAction();
    virtual ~Watch();
    Watch(const Watch& other);
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual PrintActionsLog* dupAction();
    virtual ~PrintActionsLog();
    PrintActionsLog(const PrintActionsLog& other);
};

class Exit : public BaseAction {
public:
    Exit();
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual Exit* dupAction();
    virtual ~Exit();
    Exit(const Exit& other);
};
#endif
