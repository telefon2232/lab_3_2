#include "postfix.h"
#include <vector>
#include <stack>
#include <iostream>

using namespace std;

bool TPostfix::IsOperator(char v)
{
	if (v == '~' || v == '-' || v == '+' || v == '/' || v == '*' || v == '^' || v == '(' || v == ')')
		return true;
	else
		return false;
}
bool TPostfix::IsNumeral(char v)
{
	if (int(v) >= 48 && int(v) <= 57)
		return true;
	return false;
}

bool TPostfix::CheckOperations()
{
	for(int i = 0; i < fixion.size(); i++)
		if(!IsNumeral(fixion[i]) && !IsOperator(fixion[i]))
			return false;
	int Operands = 0, Operations = 0;
	for (int i = 0; i < fixion.size(); i++)
	{
		if (fixion[i] == '(' || fixion[i] == ')')
		{
			i++;
			if (i == fixion.size())
				break;
		}

		if (fixion[i] == '-' || fixion[i] == '+' || fixion[i] == '/' || fixion[i] == '*' || fixion[i] == '^')
			Operations++;

		if (!IsOperator(fixion[i]))
		{
			while (!IsOperator(fixion[i]))
			{
				i++;
				if (i == fixion.size())
					break;
			}
			i--;
			Operands++;
		}
	}
	if (Operands == Operations + 1)
		return true;
	else return false;
}

bool TPostfix::CheckBrackets()
{
	int LeftBrackets = 0, RightBrackets = 0;
	for (int i = 0; i < fixion.size(); i++)
	{
		if (fixion[i] == '(')
			LeftBrackets++;
		else if (fixion[i] == ')')
			RightBrackets++;
		if (RightBrackets > LeftBrackets)
			throw "Error Brackets";
	}
	if (LeftBrackets == RightBrackets)
		return true;
	else return false;
}

int TPostfix::Priority(char a, char b)
{
	if (a == '~')
		return 1;
	if (b == '~')
		return -1;
	if (a == '^' && b != '~')
		return 1;
	if (b == '^' && a != '~')
		return -1;
	if ((a == '*' || a == '/') && (b == '*' || b == '/') || (a == '+' || a == '-') && (b == '+' || b == '-'))
		return 0;
	if ((a == '*' || a == '/') && (b == '+' || b == '-'))
		return 1;
	if ((a == '+' || a == '-') && (b == '*' || b == '/'))
		return -1;
	throw "not operation";
}

void TPostfix::SplitStr(string& str)
{
	if (str.empty())
		return;
	if (str.back() != ' ')
		str += ' ';
}

string TPostfix::ToPostfix()
{
	if (!CheckBrackets())
		throw "Error Brackets";
	for (int j = 0; j < fixion.size(); j++)
	{
		if (fixion[0] == '-')
			fixion[0] = '~';
		if (fixion[j] == '-' && (fixion[j-1] == '(' || fixion[j - 1] == '~'))
			fixion[j] = '~';
	}
	int count = -1;
	stack<char> Oper;
	for (int i = 0; i < fixion.size(); i++)
	{
		if (IsOperator(fixion[i]))
		{
			if ((Oper.empty() || fixion[i] == '(') && fixion[i]!='~')
			{
				if (fixion[i] != '(')
					SplitStr(postfix);
				Oper.push(fixion[i]);
				count++;
				continue;
			}
			else if ((Oper.empty() || Oper.top() == '~') && fixion[i] == '~')
			{
				Oper.push(fixion[i]);
				count++;
				continue;
			}
			else if (fixion[i] == ')')
			{
				while (Oper.top() != '(')
				{
					SplitStr(postfix);
					postfix += Oper.top();
					Oper.pop();
					count--;
				}
				Oper.pop();
				count--;
			}
			else if ((Oper.top()) == '(' || Priority(fixion[i], Oper.top()) == 1)
			{
				Oper.push(fixion[i]);
				SplitStr(postfix);
				count++;
			}
			else if (Priority(fixion[i], Oper.top()) == -1)
			{
				SplitStr(postfix);
				while (!(Oper.empty()))
				{
					if (Oper.top() == '(')
						break;
					postfix += Oper.top();
					Oper.pop();
					SplitStr(postfix);
					count--;
				}
				Oper.push(fixion[i]);
				count++;
			}
			else if (Priority(fixion[i], Oper.top()) == 0)
			{
				SplitStr(postfix);
				postfix += Oper.top();
				Oper.pop();
				Oper.push(fixion[i]);
				SplitStr(postfix);
			}
		}
		else
			postfix += fixion[i];
		if (i == fixion.size() - 1)
		{
			for (count; count >= 0; count--)
			{
				SplitStr(postfix);
				postfix += Oper.top();
				Oper.pop();
			}
		}
	}
	return postfix;
}

double TPostfix::Calculate()
{
	if (!CheckOperations() || postfix == "")
		throw "Error Operand";
	stack<double> Res;
	string str;
	for (int i = 0; i < postfix.size(); i++)
	{
		if (postfix[i] == ' ')
			continue;
		if (!IsOperator(postfix[i]))
		{
			while (postfix[i] != ' ')
			{
				str += postfix[i];
				i++;
			}
			Res.push(atof(str.c_str()));
			str = "";
		}
		else
		{
			if (postfix[i] == '~')
			{
				double a = Res.top();
				Res.pop();
				Res.push(-a);
				continue;
			}
			double a = Res.top();
			Res.pop();
			double b = Res.top();
			Res.pop();
			if (postfix[i] == '+')
				Res.push(b + a);
			else if (postfix[i] == '-')
				Res.push(b - a);
			else if (postfix[i] == '*')
				Res.push(b * a);
			else if (postfix[i] == '/')
			{
				if (a == 0.0)
					throw "Error Division";
				Res.push(b / a);
			}
			else if (postfix[i] == '^')
				Res.push(pow(b,a));
		}
	}
	return Res.top();
}