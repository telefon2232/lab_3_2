#include <string>
#include <stack>

using namespace std;

class TPostfix
{
	string fixion;
	string postfix;

	bool IsOperator(char v); 
	bool IsNumeral(char v);
	bool CheckOperations();
	bool CheckBrackets();
	void SplitStr(string& str);
	int Priority(char a, char b);
public:
	string ToPostfix();
	double Calculate();
	string Getfixion() { return fixion; }
	string GetPostfix() { return postfix; }
	TPostfix(string tmp = "")
	{
		fixion = tmp;
		postfix = "";
	}
	TPostfix(TPostfix& v)
	{
		fixion = v.fixion;
		postfix = v.postfix;
	}
};