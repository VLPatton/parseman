/*

Copyright 2023 Charlotte Patton

This file is part of Parseman.

Parseman is free software: you can redistribute it and/or modify it under the
terms of the GNU Lesser General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later 
version.

Parseman is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along 
with Parseman. If not, see <https://www.gnu.org/licenses/>.

*/

#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <typeinfo>
#include <exception>
#include <regex>
#include <iostream>
#include <cassert>

namespace parseman {

    /**
     * @class BadParserType
     * @brief This is the exception thrown by Parser::get() when it fails an 
     * RTTI type check.
     *
     * This will typically only occur from a mismatch between the template of 
     * the Parser::get() and the type specified for that command in the 
     * Parser::setType() method.
     */
    class BadParserType : public std::exception {
        public:
            const char* what() const noexcept {
                return "A bad type check has occurred! Please check the input type!\n";
            }
    };

    /**
     * @class Parser
     * @brief This class is the main Parser class. Pass argv as a vector of 
     * standard strings to the constructor.
     *
     * The template of this class should match the enum that you wish to use for
     * command switching.
     */
    template <class E>
    class Parser {
        public:
            /**
             * @brief Parser class constructor. Argv must be converted to a 
             * standard vector of standard strings and then passed to this 
             * constructor.
             *
             * The constructor will recreate the command line into a string 
             * based on this new argument vector. This allows for easier usage 
             * of regex.
             * @param argv The argument vector of type std::vector<std::string>&.
             * @todo Add a flat string overload for the Parser.
             */
            Parser(std::vector<std::string>& argv) {
                for (std::string s : argv) {
                    cmdline = cmdline + s + " ";
                }
            }

            /**
             * @brief Returns the value of the command switch, or if the command
             * switch is present based on the template provided for the type.
             *
             * The template can be any primitive typename, or std::string. If 
             * the template is a bool, this method will return whether or not a 
             * match was found for the given regex, provided by the setPattern 
             * method.
             * @param cmd The command of the type of enum that the object was 
             * templated on. Make sure to have set the pattern and type using 
             * the appropriate methods.
             * @param submatch The submatch within the regex to attempt to 
             * convert. This will depend on your regex, but the default is 2. If
             * you're having trouble with type conversions at run-time, try 
             * checking this value.
             * @return The value of type T on success; throws exception on 
             * failure.
             * @throws BadParserType
             * @see setPattern()
             * @see setType()
             * @warning Make sure that your types are all correct. Since C++ is
             * statically typed, a lot of the type checking done at runtime can 
             * only be fixed through a recompile.
             */
            template <typename T>
            T get(E cmd, int submatch = 2) {
                if (typeid(T).hash_code() != typemap[cmd] && typeid(T).hash_code() != typeid(bool).hash_code())
                    throw BadParserType{};

                T temp = T();
                std::smatch sm_result {};

                std::regex_search(cmdline, sm_result, patmap[cmd]);

                std::stringstream temp_ss {sm_result.str(submatch)};

                //std::cout << sm_result.str() << std::endl;
                
                if (typeid(T).hash_code() == typeid(bool).hash_code())
                    temp_ss >> std::boolalpha >> temp;
                else
                    assert(temp_ss >> temp);

                std::cout << temp << std::endl;

                return temp;
            }

            /**
             * @brief This method sets the regular expression to search for a 
             * specific command from the templated enum.
             *
             * The pattern is used in a regex_search rather than a match for all
             * cases. This means that your regex can be simpler but may need to 
             * be taken into consideration depending on your needs.
             * @param cmd The command of the object templated enum type, used to
             * specify which command the pattern applies to.
             * @param pattern The regular expression to search for in the 
             * command line to determine whether a command is present and what 
             * it's value is (when applicable).
             * @see get()
             * @see setType()
             */
            void setPattern(E cmd, std::string const& pattern) {
                patmap[cmd] = std::regex(pattern, std::regex::ECMAScript | std::regex::multiline);
            }

            /**
             * @brief This method sets the type of the argument to search for 
             * based on a given typeid.
             *
             * The type is very important and must be set before checking the 
             * value of a match. The get() method will likely throw a 
             * BadParserType exception that will be caused by an unknown type.
             * @param cmd The command of the object templated enum type, used to
             * specify which command the type applies to.
             * @param type The typeid() of the type. This is stored as a hash 
             * code for comparing types.
             * @warning This method must be called before using the get() method.
             * @see get()
             * @see setPattern()
             */
            void setType(E cmd, std::type_info const& type) {
                typemap[cmd] = type.hash_code();
            }

            /**
             * @brief Returns the full command line string.
             */
            std::string getCmdline() { return cmdline; }
        private:
            std::map<E, std::regex> patmap {};
            std::map<E, size_t> typemap {};
            std::string cmdline = "";
    };
}
