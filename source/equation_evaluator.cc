// Usage Rights (who can use this code):
// Usage allowed for all programs with permissions from author
//
// This notice must appear in all copies of this file and its derivatives.
//
// History of Significant Contributions (don't put commit logs here):
//    2016-03-26      Ruoyu Tan <t.ruoyu@gmail.com>

#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace
{

// A class to evaluate a set of equations and output the unsigned integer value of each variables
// Intended use:
//     // Initialize object with input .txt file path
//     eqn_eval evaluator(input_txt_file_path);
//     // Run evaluation and write the output at given output .txt file path
//     evaluator.run_evaluation(output_txt_file_path);
//
class eqn_eval
{
public:
    // Read equation at each line from input file and separate it as lhs and rhs
    // @param input_file_path input .txt file path
    eqn_eval(const std::string &input_file_path)
    {
        std::ifstream input_file(input_file_path);
        std::string line_contents;

        while (std::getline(input_file, line_contents))
        {
            build_expression(line_contents);
        }
    }

    // Separate equation as lhs and rhs and save in expression map
    // @param line_contents expression of each line
    void build_expression(const std::string &line_contents)
    {
        std::string buffer = "";
        std::string variable_name = "";

        for (uint32_t i = 0; i < line_contents.size(); i++)
        {

            if (line_contents[i] != '=')
            {

                if (line_contents[i] != ' ')
                {
                    buffer += line_contents[i];
                }
            }
            else
            {
                variable_name = buffer;
                buffer = "";
            }
        }
        expression[variable_name] = buffer;
    }

    // Run evaluation
    // @param output_file_path output .txt file path
    void run_evaluation(const std::string &output_file_path)
    {
        // Repeat evaluation until all the equations are solved
        while (result.size() < expression.size())
        {

            for (auto &it : expression)
            {
                // Check whether current equation is solved, if no, check the number of unknown variables
                if (result.find(it.first) == result.end())
                {
                    // If there is no unknown, solve the equation
                    if (check_unknowns_num(it.second) == 0)
                    {
                        add_result(it.first);
                    }
                }
            }
        }

        write_output(output_file_path);
    }

    // Check the number of unknowns in current equation
    // @param rhs the equation to be checked
    // @return the number of unknowns
    uint32_t check_unknowns_num(const std::string &rhs)
    {
        std::string buffer = "";
        uint32_t unknows_num = 0;

        for (uint32_t i = 0; i < rhs.size(); i++)
        {

            if (std::isalpha(rhs[i]))
            {
                buffer += rhs[i];
            }
            // Check previous variable if '+' or the end of equation
            if ((rhs[i] == '+' || i == rhs.size() - 1) && buffer != "")
            {
                // If the variables is unknown
                if (result.find(buffer) == result.end())
                {
                    unknows_num++;
                }

                buffer = "";
            }
        }
        return unknows_num;
    }

    // Solve the equation and add the variable in known list
    // @param variable_name variable_name
    void add_result(const std::string &variable_name)
    {
        std::string rhs = expression[variable_name];
        std::string buffer = "";
        uint32_t res = 0;

        for (uint32_t i = 0; i < rhs.size(); i++)
        {

            if (rhs[i] != '+')
            {
                buffer += rhs[i];
            }
            // add up value if '+' or the end of equation
            if (rhs[i] == '+' || i == rhs.size() - 1)
            {

                if (result.find(buffer) != result.end())
                {
                    res += result[buffer];
                }
                else
                {
                    res += std::stoi(buffer);
                }
                buffer = "";
            }
        }

        result[variable_name] = res;
    }

    void write_output(const std::string &output_file_path)
    {
        std::ofstream output_buffer(output_file_path);

        for (auto &it : result)
        {
            output_buffer << it.first << " = " << it.second << std::endl;
        }
    }

private:
    // A map stores all the equation as <lhs,rhs> pattern
    std::unordered_map<std::string, std::string> expression;
    // A map stores all the known variable
    std::map<std::string, uint32_t> result;
};
}

int main(int argc, char *argv[])
{
	try
	{
	    if (argc!=3) throw std::runtime_error("Incorrect Input");
        const std::string input_file_path = argv[1];
        const std::string output_file_path = argv[2];
        eqn_eval evaluator(input_file_path);
        evaluator.run_evaluation(output_file_path);
    }
    catch (std::exception& e)
    {
    	std::cout<<" Fail reason: "<<e.what()<<std::endl;
    }
}