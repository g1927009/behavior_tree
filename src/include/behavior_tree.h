#ifndef BEHAVIOR_TREE_H
#define BEHAVIOR_TREE_H

#include <vector>
#include <map>
#include <string>
#include <memory>

// --------------------------
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

class Composite : public BTnode
{
private:
	std::vector<BTnode*> children;
public:
	Composite() {}
	virtual ~Composite() {}

	bool add_child(BTnode* p) { children.push_back(p); return true; }

	virtual State update()
	{
		for (auto& chile: children) {
			chile->update();
		}
	}
};

class Sequence : public Composite
{
public:
	Sequence() {}
	virtual ~Sequence() {}
};

class Selector : public Composite
{
public:
	Selector() {}
	virtual ~Selector() {}
};

// --------------------------
class BehaviorTree
{
	BTnode* pRoot = nullptr;
	BlackBoard black_board;
public:
	BehaviorTree() { BTnode::set_blackboard(&black_board); }
	~BehaviorTree() {}

	BTnode::State update()
	{
		if (!pRoot) return BTnode::State::not_ready;
		if (pRoot->get_state() == BTnode::State::compleated) return BTnode::State::compleated;

		return pRoot->update();
	}
};

#endif //BEHAVIOR_TREE_H
