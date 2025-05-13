#pragma once

#include "Actor.h"
class Actor;

class State
{
public:
	State() {}
	virtual ~State() {}

    virtual int32 GetStateType() abstract;
    virtual AnimType GetAnimType() abstract;

    virtual void Enter() {} // ���� ���� �� ȣ��
    virtual void Update(float deltaTime) {} // ���� ������Ʈ
    virtual void Exit() {} // ���� ���� �� ȣ��

    virtual bool IsEnd() { return false; }
    virtual bool OnLeftClickEvent(int32 x, int32 y) { return false; }
};

class StateMachine
{
public:
    StateMachine() : _currentState(nullptr) {}
    ~StateMachine() {}

    void Update(float deltaTime)
    {
        if (_nextStateType != -1)
        {
            changeState(_nextStateType);
            _nextStateType = -1;
        }

        if (_currentState)
        {
            _currentState->Update(deltaTime); // ���� ���� ������Ʈ
         
            if (_currentState->IsEnd())
            {
                ReserveNextState(_defaultState);
                PrintLog(std::format(L"@@ Reserve Default State : {0}", _defaultState));
            }
        }
    }

    void AddState(State* state)
    {
        if (state == nullptr)
            return;

        if (_allStates.find(state->GetStateType()) != _allStates.end())
        {
            return;
        }

        _allStates.emplace(state->GetStateType(), state);
    }

    void SetDefaultState(int32 defaultState) { _defaultState = defaultState; }
    void ReserveNextState(int32 nextState)
    {
        PrintLog(std::format(L"@@ Reserve State : {0}", nextState));

        _nextStateType = nextState; 
    }
    AnimType GetCurrStateAnimType()
    {
        if (_currentState)
        {
            return _currentState->GetAnimType();
        }
        return AnimType::A_IDLE;
    }

    bool OnLeftClickEvent(int32 x, int32 y);

private:
    void changeState(int32 stateType)
    {
        // ���� state ��ȯ�̸� ����
        if (_currentState && _currentState->GetStateType() == stateType)
            return;

        if (_currentState)
            _currentState->Exit(); // ���� ���� ����

        auto find = _allStates.find(stateType);
        if (find == _allStates.end())
            return;

        State* newState = find->second;
        _currentState = newState;

        PrintLog(std::format(L"@@ Change State : {0}", newState->GetStateType()));

        if (_currentState)
            _currentState->Enter(); // ���ο� ���� ����
    }

private:
    State* _currentState;           // ���� ����
    int32 _nextStateType = -1;           // ���� ����� ����

    int32 _defaultState = -1;       // �⺻ ����
    map<int32, State*> _allStates;  // ��� State
};