#ifndef BEHAVIOR_TREE_H
#define BEHAVIOR_TREE_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <memory>

// --------------------------
// 共通掲示板
class BlackBoard
{
private:
	std::map<std::string, int> mp;

public:

	void set(std::string key, int value) {mp[key] = value;}
	int get(std::string key) { return mp[key]; }

	bool remove(std::string key) {
		auto itr = mp.find(key);
		if (itr == mp.end()) return false;
		mp.erase(itr);
	}

	bool is_exist(std::string key) const {return (mp.find(key) != mp.end());}
};


// --------------------------
// コンポーネントとしてノードに持たれる
class Decorator
{
public:
	Decorator() {}
	~Decorator() {}
};


// --------------------------
// ノードの基底
class BTnode
{
public:
	enum class State {
		not_ready,
		ready,
		running,
		compleated,// succeeded やfailedを返した後の内部状態
		succeeded,
		failed,
	};

private:
	std::vector<Decorator*> decorators;
	BTnode* pChild = nullptr;
	State state = State::ready;

	static BlackBoard* pBlackBoard;
public:
	BTnode() {}
	virtual ~BTnode() {}

	virtual bool add_child(BTnode* p) { if (pChild != nullptr) return false; pChild = p; return true; }
	void add_decorator(Decorator* p) { decorators.push_back(p); }
	virtual State update() = 0;

	State get_state() const { return state; }

	static void set_blackboard(BlackBoard* p) { pBlackBoard = p; }
};

// --------------------------
// 具体的なAIの行動（メッセージを表示する）
class MessageAction : public BTnode
{
	std::string text = "";
public:
	MessageAction() {}
	~MessageAction() {}

	void set_message(const std::string& str) { text = str; }

	State update()
	{
		std::cout << text << std::endl;

		return State::succeeded;
	}
};

class Composite : public BTnode
{
private:
	std::vector<BTnode*> children;
	int current = 0;// 途中から再開するためにインデックスを確保している
public:
	Composite() {}
	virtual ~Composite() {}

	bool add_child(BTnode* p) { children.push_back(p); return true; }

	virtual State update()
	{
		if (-1 == current) return State::compleated;

		//　先頭から順番に実行する
		for (unsigned int i = current; i < children.size(); i++ ) {
			current = i;
			State s = children[i]->update();
			if (s == State::running) return State::running;
		}

		current = -1;// 終了フラグ

		return State::succeeded;
	}
};

class Sequence : public Composite
{
public:
	Sequence() {}
	~Sequence() {}
};

class Selector : public Composite
{
public:
	Selector() {}
	~Selector() {}
};

// --------------------------
class BehaviorTree
{
	Sequence root;
	BlackBoard black_board;
public:
	BehaviorTree() { BTnode::set_blackboard(&black_board); }
	~BehaviorTree() {}

	bool add_child(BTnode* p)
	{
		return root.add_child(p);
	}

	BTnode::State update()
	{
		if (root.get_state() == BTnode::State::compleated) return BTnode::State::compleated;

		return root.update();
	}
};

#endif //BEHAVIOR_TREE_H
