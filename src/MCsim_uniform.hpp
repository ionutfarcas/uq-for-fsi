#ifndef MCSIMUNIFORM_HPP_
#define MCSIMUNIFORM_HPP_

#include "UQsim.hpp"
#include "uniformRV.hpp"
#include "helper.hpp"

class MCSimulation_uniform : public UQSimulation
{
private:
	int nsamples;
	int nprocs;
	int rank;
	double mean;
	double std_dev;
	double rho_f_p1;
	double rho_f_p2;
	double nu_f_p1;
	double nu_f_p2;
	double rho_s_p1;
	double rho_s_p2;
	std::string nastin_dat;
	std::string solidz_dat;
	std::string run_exec;
	std::string output_data;
	std::string gather_data_exec_mc;
	std::string postproc_stat_exec_mc;
	std::string postproc_file_all_mc;
	std::string postproc_stat_mc;
	std::string insert_nastin_exec;
	std::string insert_solidz_exec;

	UniformRandomVariable urv;

	int local_global_mapping(const int local_index, const int& rank) const
	{
		int global_id;
		int local_samples;

		local_samples = nsamples/nprocs;
		global_id = local_index + rank*local_samples;

		return global_id;
	}

public:
	MCSimulation_uniform() {}

	MCSimulation_uniform(
		const int& _rank, 
		const int& _nprocs, 
		const int& _nsamples, 
		const double& _mean, 
		const double& _std_dev)
	{
		assert(_nsamples >= _nprocs);

		rank = _rank;
		nprocs = _nprocs;
		nsamples = _nsamples;
		mean = _mean;
		std_dev = _std_dev;
	}

	MCSimulation_uniform(
		std::string& _nastin_dat, 
		std::string& _solidz_dat, 
		std::string& _run_exec,
		std::string& _output_data, 
		std::string& _gather_data_exec_mc, 
		std::string& _postproc_stat_exec_mc,  
		std::string& _postproc_file_all_mc, 
		std::string& _postproc_stat_mc, 
		std::string& _insert_nastin_exec, 
		std::string& _insert_solidz_exec, 
		const unsigned int& _nsamples, 
		const double& _rho_f_p1,  
		const double& _rho_f_p2, 
		const double& _nu_f_p1, 
		const double& _nu_f_p2, 
		const double& _rho_s_p1, 
		const double& _rho_s_p2)
	{
		nastin_dat = _nastin_dat;
		solidz_dat = _solidz_dat;
		run_exec = _run_exec;
		output_data = _output_data;
		gather_data_exec_mc = _gather_data_exec_mc;
		postproc_stat_exec_mc = _postproc_stat_exec_mc;
		postproc_file_all_mc = _postproc_file_all_mc;
		postproc_stat_mc = _postproc_stat_mc;
		insert_nastin_exec = _insert_nastin_exec;
		insert_solidz_exec = _insert_solidz_exec;

		nsamples = _nsamples;

		rho_f_p1 = _rho_f_p1;
		rho_f_p2 = _rho_f_p2;
		nu_f_p1 = _nu_f_p1;
		nu_f_p2 = _nu_f_p2;
		rho_s_p1 = _rho_s_p1;
		rho_s_p2 = _rho_s_p1;
	}

	virtual int data_decomp() const
	{
		int local_points = 0, rem = 0;

		local_points = nsamples/nprocs;
		rem = nsamples % nprocs;

		if (rank == nprocs - 1)   
		{
			local_points += rem;
		}

		return local_points;
	}

	virtual std::vector<double> pre_processing() const
	{
		std::vector<double> samples;

		samples = urv.get_samples(mean, std_dev, nsamples);

		return samples;
	}

	virtual std::vector<double> pre_processing(const double& param1, const double& param2) const
	{
		std::vector<double> samples;

		samples = urv.get_samples(param1, param2, nsamples);

		return samples;
	}

	virtual void simulation(std::vector<double>& pre_proc_result) const  
	{
		std::string modify_nastin_data;
		std::string get_all_data;

		int modify_nastin_data_ok = 0;

		int run_ok = 0;
		int get_all_data_ok = 0;

		for(int i = 0 ; i < nsamples ; ++i)
		{
			modify_nastin_data = run_insert_nastin_1d(insert_nastin_exec, nastin_dat, pre_proc_result[i]);
				
			modify_nastin_data_ok = system(modify_nastin_data.c_str());
			assert(modify_nastin_data_ok >= 0);

			run_ok = system(run_exec.c_str());
			assert(run_ok >= 0);

			get_all_data = run_gather_data(gather_data_exec_mc, output_data, postproc_file_all_mc, i+1);

			get_all_data_ok = system(get_all_data.c_str());
			assert(get_all_data_ok >= 0);
		}
	}
	
	virtual void post_processing() const
	{
		std::string get_postproc_stat;
		int get_postproc_stat_ok = 0;

		get_postproc_stat = run_postproc_stat(postproc_stat_exec_mc, postproc_file_all_mc, postproc_stat_mc);

		get_postproc_stat_ok = system(get_postproc_stat.c_str());
		assert(get_postproc_stat_ok >= 0);
	}
	
	~MCSimulation_uniform() {}
};

#endif /* MCSIMUNIFORM_HPP_ */