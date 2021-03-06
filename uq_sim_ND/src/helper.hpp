#ifndef HELPER_HPP_
#define HELPER_HPP_

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <chrono>

#include "sgpp_base.hpp"

typedef std::vector<std::vector<int>> vec2d_int;
typedef std::vector<std::vector<double>> vec2d_double;
typedef std::vector<std::vector<SGPP::float_t>> vec2d_float_t;

std::string run_insert_nastin_2d(const std::string nastin_exec, const std::string nastin_data, 
	double& new_density, double& new_viscosity);

std::string run_insert_nastin_1d(const std::string nastin_exec, const std::string nastin_data, 
	double& new_viscosity);

std::string run_insert_solidz_1d(const std::string solidz_exec, const std::string solidz_data, 
	double& new_density);

std::string run_gather_data(const std::string gather_data_exec, const std::string datafile, 
	const std::string datafile_all, const int& id);

std::string run_postproc_stat(const std::string postproc_stat, const std::string all_data, 
	const std::string stats, const int& no_of_simulations);

std::string run_get_output(const std::string get_output, const std::string data);

std::string run_gather_alya_output(const std::string get_alya_output, const int& run_id);

int parse_configfile(const std::string& config_file_name,
	std::string& nastin_dat,
	std::string& solidz_dat,
	std::string& run_exec,
	std::string& output_data,
	std::string& gather_data_exec_mc,
	std::string& postproc_stat_exec_mc,
	std::string& postproc_file_all_mc,
	std::string& postproc_stat_mc,
	std::string& gather_data_exec_sc,
	std::string& get_output_sc,
	std::string& postproc_stat_exec_sc,
	std::string& output_file_sc,
	std::string& coeff_sc,
	std::string& postproc_stat_sc,
	std::string& insert_nastin_exec,
	std::string& insert_solidz_exec,
	std::string& gather_alya_output,
	unsigned int& uq_method,
	unsigned int& pdf,
	unsigned int& nsamples,
	unsigned int& ncoeff,
	unsigned int& prob_dim,
	unsigned int& sg_level,
	double& rho_f_p1,
	double& rho_f_p2,
	double& nu_f_p1,
	double& nu_f_p2,
	double& rho_s_p1,
	double& rho_s_p2);

void get_output_data(const std::string get_output_sc, int& no_of_datapoints, std::vector<double>& disp_x, 
	std::vector<double>& force0, std::vector<double>& force1);

int save_coeff(const std::string file_name, const double& disp_x, const double& force0, const double& force1);

int l1_norm(const std::vector<int> mindex);

#endif /* HELPER_HPP_ */