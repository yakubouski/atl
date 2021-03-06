#pragma once
#include <unordered_set>
#include <vector>
#include <array>
#include <cstring>
#include <string>
#include <getopt.h>

namespace atl {
	namespace sys {
		using namespace std;

		class command_line {
		public:
			class option {
			protected:
				friend class command_line;
				const char		short_name;
				const char*		long_name;
				int				required;
				const char*		value;
				operator struct ::option() const { return { long_name,required ,nullptr,short_name }; }
			public:
				option(const char* long_name, int required = 0, const char short_name = 0) : short_name(short_name), long_name(long_name), required(required), value(nullptr) { ; }
				option(const char* long_name, const char short_name, int required, const char* value) : short_name(short_name), long_name(long_name), required(required), value(value) { ; }

				class hash {
				public:
					inline size_t operator()(const option& opt) const {
						return std::hash<string>()(opt.long_name);
					}
				};
				class equal {
				public:
					bool operator()(const option & o1, const option & o2) const {
						return strcmp(o1.long_name, o2.long_name) == 0;
					}
				};
			};

			typedef std::unordered_set<option, option::hash, option::equal>	options_set;

		protected:
			options_set		options_list;
			string			program_name;
		public:
			/*
			* Define options and parse command line
			*/
			template<typename ... ARGS>
			void options(int argc, char* argv[], ARGS&& ... args) throw (int) {
				vector<struct ::option> options_long = { args...,{0,0,nullptr,0} };
				array<int, 256>	options_map;
				string options_short("");
				int index = 0;
				options_map.fill(0);
				program_name = argv[0];

				for (auto&& it : options_long) {
					if (it.val) {
						options_short += (char)it.val;
						switch (it.has_arg) {
						case 2:
							options_short += ':';
						case 1:
							options_short += ':';
							break;
						}
					}
					options_map[it.val] = index++;
				}

				int opt = -1, option_index = -1;
				while ((opt = getopt_long(argc, argv, options_short.c_str(), options_long.data(), &option_index)) != -1) {
					if (opt == 0) {
						auto && o = options_long[option_index];
						options_list.emplace(o.name, (char)o.val, o.has_arg, optarg);
						if (o.has_arg == 1 && optarg == nullptr) {
							throw option_index;
						}
					}
					else if (opt != '?') {
						auto && o = options_long[options_map[opt]];
						options_list.emplace(o.name, (char)o.val, o.has_arg, optarg);
						if (o.has_arg == 1 && optarg == nullptr) {
							throw options_map[opt];
						}
					}
				}
			}
			/*
			* Get program name
			*/
			const char* program() const {
				auto it = program_name.find_last_of('/');
				return program_name.c_str() + (it != string::npos ? it + 1 : 0);
			}
			/*
			* Get program working directory
			*/
			const char* pwd() const {
				return std::getenv("PWD");
			}
			/*
			* Get program executable string
 			*/
			const char* exec_name() const {
				return program_name.c_str();
			}
			/*
			* Check options exist
			* @return bool
			*/
			bool is(const char* long_name) const {
				return options_list.find({ long_name }) != options_list.end();
			}
			/*
			* Get parsed command line option value
			* @return int
			*/
			int operator() (const char* long_name, int default_value) const {
				auto it = options_list.find({ long_name });
				if (it != options_list.end()) {
					return std::atoi(it->value);
				}
				return default_value;
			}
			/*
			* Get parsed command line option value
			* @return double
			*/
			double operator() (const char* long_name, double default_value) const {
				auto it = options_list.find({ long_name });
				if (it != options_list.end()) {
					return std::atof(it->value);
				}
				return default_value;
			}
			/*
			* Get parsed command line option value
			* @return const char*
			*/
			const char* operator() (const char* long_name, const char* default_value) const {
				auto it = options_list.find({ long_name });
				if (it != options_list.end()) {
					return it->value;
				}
				return default_value;
			}
		};
	}
}
