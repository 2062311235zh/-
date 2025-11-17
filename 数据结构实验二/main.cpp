#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
#include<cstdlib>
using namespace std;
vector<int> KMP(const string& target, const string& pattern_string);
void display_result(const string& target_string, const int pattern_string_size, const vector<int>& kmp_result);
int main(int argc, char* argv[])
{
    if (argc != 3 && argc != 4)
    {
        cout << "input numbers error!";
        exit(1);
    }
    //string command(argv[1]), find(argv[2]);
    //cout << argv[0] << " " << argv[1] << " " << find << endl;
    if (string(argv[1]) == "-d")//显示结果
    {
        string result_txt_path(string(argv[2]) + ".txt"), result_html_path(string(argv[2]) + ".html");
        ifstream result_txt(result_txt_path);
        if (!result_txt)//文件不存在
        {
            cout << "can not open this document";
            return 1;
        }
        /*system(result_html_path.c_str());*/

        string data_txt_path;
        getline(result_txt, data_txt_path);
        
        ifstream data_txt(data_txt_path, ios::in);
        if (!data_txt.is_open())
        {
            cout << "data can not open!";
            exit(1);
        }

        int pattern_size;
        result_txt >> pattern_size;
        vector<int> kmp_result;
        while (!result_txt.eof())
        {
            int location;
            result_txt >> location;
            kmp_result.push_back(location);
        }
        
        string target_string;
        ostringstream ss;
        ss << data_txt.rdbuf();
        target_string = ss.str();
        data_txt.close();
        
        display_result(target_string, pattern_size, kmp_result);
        //在命令行输出结果结束

        /*ostringstream ss;
        ss << result.rdbuf();
        string output = ss.str();
        for (int location = 0; location < output.size(); location++)
        {
            cout << output[location];
        }*/


        //int color = 0, result_location = 0;
        //for (int i = 0; i < result.size(); i++)
        //{
        //    if (result_location != -1 && i == result[result_location])
        //    {
        //        color = find.size();
        //        result_location++;
        //        if (result_location == result.size())
        //        {
        //            result_location = -1;
        //        }
        //    }
        //    if (color)
        //    {
        //        color--;
        //        cout << "\033[43m";//黄色背景
        //    }
        //    else
        //    {
        //        cout << "\033[0m";//原背景
        //    }
        //    cout << result[i];
        //}
        //cout << "\033[0m";//原背景



        result_txt.close();
    }
    else//读入
    {
        string data_txt_path(argv[1]), pattern_string(argv[2]);
        ifstream target(data_txt_path, ios::in);
        if (!target.is_open())
        {
            cout << "target can not open!";
            exit(1);
        }
        string target_string;
        /*string line;
        while (getline(target, line)) {
            target_string += line;
            target_string += '\n';
        }*/

        /*std::ifstream target(data_txt_path);
        std::string target_string((istreambuf_iterator<char>(target)),
                           istreambuf_iterator<char>());*/
        ostringstream ss;
        ss << target.rdbuf();
        target_string = ss.str();
        target.close();

        vector<int> kmp_result = KMP(target_string, pattern_string);


        display_result(target_string, pattern_string.size(), kmp_result);


        if (argc == 4)//保存结果
        {
            ofstream result_for_html(string(argv[3]) + ".html"), result_for_txt(string(argv[3]) + ".txt");

            if (!result_for_html.is_open() || !result_for_txt.is_open())
            {
                cout << "deposit can not open!";
                exit(1);
            }

            result_for_txt << data_txt_path << endl << pattern_string.size() << " ";
            for (int i = 0; i < kmp_result.size(); i++)
            {
                result_for_txt << kmp_result[i] << " ";
            }
            result_for_txt << endl /*<< target_string*/;
            result_for_txt.close();

            result_for_html << "<!DOCTYPE html>\n"
                << "<html>\n"
                << "<head>\n"
                << "</head>\n"
                << "<body>\n"
                << "find result for \"" << pattern_string << "\":<br>\n";

            int color = 0, kmp_number_location = 0;
            bool background_color = 0;
            for (int i = 0; i < target_string.size(); i++)
            {
                if (kmp_number_location != -1 && i == kmp_result[kmp_number_location])
                {
                    color = pattern_string.size();
                    kmp_number_location++;
                    if (kmp_number_location == kmp_result.size())
                    {
                        kmp_number_location = -1;
                    }
                }

                if (color)
                {
                    color--;
                    if (!background_color)
                    {
                        result_for_html << "<span style = \"background: yellow\">";
                        background_color = 1;
                    }
                }
                else if (!color && background_color)
                {
                    result_for_html << "</span>";
                    background_color = 0;
                }
                if (target_string[i] == '\n')
                {
                    result_for_html << "<br>\n";
                }
                else
                {
                    result_for_html << target_string[i];
                }
            }
            result_for_html << "</body>\n"
                << "</html>";
            result_for_html.close();
        }
    }
}
vector<int> KMP(const string& target, const string& pattern_string)
{
    //get_nextval
    int* next_val = new int[pattern_string.size()] {-1};
    int compare_location = -1;
    vector<int> result;
    for (int location = 0; location < pattern_string.size() - 1;)
    {
        if (compare_location == -1 || pattern_string[compare_location] == pattern_string[location])
        {
            location++;
            compare_location++;
            if (pattern_string[compare_location] != pattern_string[location])
            {
                next_val[location] = compare_location;
            }
            else
            {
                next_val[location] = next_val[compare_location];
            }
        }
        else
        {
            compare_location = next_val[compare_location];
        }
    }

    int target_location = 0, pattern_location = 0;
    while (target_location < target.size())
    {
        if (pattern_location == -1 || target[target_location] == pattern_string[pattern_location])
        {
            target_location++;
            if (pattern_location == pattern_string.size() - 1)
            {
                pattern_location = 0;
                result.push_back(target_location - pattern_string.size());
            }
            else
            {
                pattern_location++;
            }
        }
        else
        {
            pattern_location = next_val[pattern_location];
        }
    }
    delete[] next_val;
    return result;
}
void display_result(const string& target_string,const int pattern_string_size, const vector<int>& kmp_result)
{
    int color = 0, kmp_number_location = 0;
    bool background_color = 0;//1 is yellow,0 is normal
    for (int i = 0; i < target_string.size(); i++)
    {
        if (kmp_number_location != -1 && i == kmp_result[kmp_number_location])
        {
            color = pattern_string_size;
            kmp_number_location++;
            if (kmp_number_location == kmp_result.size())
            {
                kmp_number_location = -1;
            }
        }


        if (color)
        {
            color--;
            if (!background_color)
            {
                cout << "\033[43m";//黄色背景
                background_color = 1;
            }
        }
        else if (!color && background_color)
        {
            cout << "\033[0m";//原背景
            background_color = 0;
        }
        cout << target_string[i];
    }
    cout << "\033[0m";//原背景
    //在命令行输出结果结束
}