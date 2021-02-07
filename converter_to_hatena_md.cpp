#include <iostream>
#include <string>
#include <fstream>

using namespace std;

/*
Katexを使うことを前提とする。
*, [, ], (, ) の6種類の文字に先頭に\とつけてエスケープする。
*/

//$$であるかを識別する
bool is_one_line_delimiter(string &s, int idx)
{
	if (s[idx] == '$' && idx + 1 < s.size() && s[idx + 1] == '$')
	{
		if (idx - 1 >= 0 && s[idx - 1] != '\\')
			//直前がエスケープシーケンスでもなければ
			return true;
		else if (idx == 0)
			//もしバッファの1文字目なら、エスケープされた$ということはない。
			return true;
	}
	return false;
}
//$であるかを識別する
bool is_in_the_text_delimiter(string &s, int idx)
{
	if (idx + 1 < s.size())
	{
		if (s[idx] == '$' && s[idx + 1] != '$')
		{
			if (idx - 1 >= 0 && s[idx - 1] != '\\')
				return true;
			else if (idx == 0)
				return true;
		}
		return false;
	}
	else
	{
		if (s[idx] == '$')
			return true;
		return false;
	}
}

int main()
{
	ofstream ofs("result.md");

	string buf;
	bool in_one_line = false; //一行モードの数式に入っているか
	bool in_the_text = false; //文中に数式埋め込むモードに入っているか
	while (getline(std::cin, buf))
	{
		for (int i = 0; i < buf.size(); i++)
		{
			if (!in_one_line && !in_the_text)
			{
				//数式モードでない場合
				if (is_one_line_delimiter(buf, i))
				{
					//一行モードの数式のスタート
					ofs << buf[i];
					//2つぶんの$$があるのでiをもう1つ増やす。
					i++;
					in_one_line = true;
				}
				else if (is_in_the_text_delimiter(buf, i))
				{
					//文中モードの数式のスタート
					in_the_text = true;
				}
			}
			else
			{
				//数式モードに属してる時

				//数式モードの終わりか？
				if (in_one_line && is_one_line_delimiter(buf, i))
				{
					//数式モードの終わりには空白を挟む　無駄なはてなリンクワードを防ぐため
					ofs << ' ';
					ofs << buf[i];
					i++;
					in_one_line = false;
				}
				else if (in_the_text && is_in_the_text_delimiter(buf, i))
				{
					//数式モードの終わりには空白を挟む　無駄なはてなリンクワードを防ぐため
					ofs << ' ';
					in_the_text = false;
				}
				else
				{
					//文中にある_, *, [, ], (, )はエスケープシーケンスをつける
					if (buf[i] == '*' || buf[i] == '[' || buf[i] == ']' || buf[i] == '(' || buf[i] == ')')
					{
						ofs << '\\';
					}

					//文中にある_, ^は前後に半角空白を挟む
					if (buf[i] == '_' || buf[i] == '^')
					{
						ofs << ' ';
					}
				}
			}

			ofs << buf[i];

			//数式モードの_や^の両隣りには半角空白を挟む
			if ((in_one_line || in_the_text) && (buf[i] == '_' || buf[i] == '^'))
			{
				ofs << ' ';
			}

			//数式モードを始めたら、無駄なはてなワードリンクを防ぐために1つ半角空白を空ける
			if ((in_one_line || in_the_text) && (buf[i] == '$'))
			{
				//数式モードの始まりは半角空白を挟む　はてなワードでのリンクを防ぐため
				ofs << ' ';
			}
		}
		//改行
		ofs << endl;
	}
	ofs.close();
	return 0;
}