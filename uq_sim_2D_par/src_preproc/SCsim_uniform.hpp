#ifndef SCSIMUNIFORM_HPP_
#define SCSIMUNIFORM_HPP_

#include "UQsim.hpp"
#include "gauss_legendre_quad.hpp"

class SCSimulation_uniform : public UQSimulation
{
private:
	int ncoeff;
	int quad_degree;
	int ncoeff_2dim;

	int quad_degree_2D;

	int dim;
	
	vec2d_int multi_index_dim2;
	GaussLegendreQuadrature glq;

	int factorial(const int& n) const
	{
		int fact = 0;

		if(n == 0)
		{
			fact = 1;
		}
		else
		{	
			fact = n*factorial(n-1);
		}

		return fact;
	}

	vec2d_int mindex(const int& degree) const
	{
		int j = 0;
		int norm = 0;

		std::vector<int> temp(dim, 0);
		vec2d_int mindex_degree;

		while(true)
		{
			norm = l1_norm(temp);

			if(norm == degree)
			{
				mindex_degree.push_back(temp);
			}

			for(j = dim - 1 ; j >= 0 ; --j)
			{
				if(++temp[j] <= degree)
					break;
				else
					temp[j] = 0;
			}

			if( j < 0)
				break;
		}

		return mindex_degree;
	}

	vec2d_int multi_index() const
	{
		int size = 0;
		vec2d_int m_index_level;
		vec2d_int result;

		for(int i = 0 ; i < ncoeff; ++i)
		{
			m_index_level = this->mindex(i);
			size = m_index_level.size();

			for(int j = size - 1; j >= 0 ; --j)
			{
				result.push_back(m_index_level[j]);
			}
		}

		return result;
	}

	int compute_no_coeff() const
	{
		int no_coeff = 1;

		for(int i = 0 ; i < dim ; ++i)
		{
			no_coeff *= (ncoeff + i);
		}

		no_coeff = no_coeff/this->factorial(dim);

		return no_coeff;
	}

	double multi_orthogonal_poly(const double& val1, const double& val2, const int& index) const
	{
		double multi_ortho_poly = 0.0;
		multi_ortho_poly = glq.orthogonal_poly(multi_index_dim2[index][0], val1)*glq.orthogonal_poly(multi_index_dim2[index][1], val2)/(glq.norm_factor(multi_index_dim2[index][0])*glq.norm_factor(multi_index_dim2[index][1]));			

		return multi_ortho_poly;
	}

public:
	SCSimulation_uniform() 
	{
		ncoeff = 0;
		quad_degree = 0;
		ncoeff_2dim = 0;
		quad_degree_2D = 0;

		dim = 0;
		multi_index_dim2 = {{0}};
	}

	SCSimulation_uniform(std::string& _nastin_dat, 
		std::string& _solidz_dat,
		std::string& _create_data_point,  
		std::string& _run_exec, 
		std::string& _output_data, 
		std::string& _gather_data_exec_sc, 
		std::string& _get_output_sc,
		std::string& _postproc_stat_exec_sc,
		std::string& _output_file_sc, 
		std::string& _coeff_sc,
		std::string& _postproc_stat_sc,
		std::string& _insert_nastin_exec, 
		std::string& _insert_solidz_exec,
		std::string& _gather_alya_output,  
		const unsigned int& _ncoeff, 
		const unsigned int& _quad_degree,
		const double& _rho_f_p1, 
		const double& _rho_f_p2, 
		const double& _nu_f_p1, 
		const double& _nu_f_p2, 
		const double& _rho_s_p1, 
		const double& _rho_s_p2,
		const double& _E_s_p1,
    	const double& _E_s_p2,
    	const double& _nu_s_p1,
    	const double& _nu_s_p2) 
	{
		ncoeff = _ncoeff;
		quad_degree = _quad_degree;

		dim = 2;

		multi_index_dim2 = multi_index();
		ncoeff_2dim = this->compute_no_coeff();

		nastin_dat = _nastin_dat;
		solidz_dat = _solidz_dat;
		create_data_point = _create_data_point;
		run_exec = _run_exec;
		output_data = _output_data;
		gather_data_exec_sc = _gather_data_exec_sc;
		get_output_sc = _get_output_sc;
		postproc_stat_exec_sc = _postproc_stat_exec_sc;
		output_file_sc = _output_file_sc;
		coeff_sc = _coeff_sc;
		postproc_stat_sc = _postproc_stat_sc;
		insert_nastin_exec = _insert_nastin_exec;
		insert_solidz_exec = _insert_solidz_exec;
		gather_alya_output = _gather_alya_output;

		quad_degree_2D = quad_degree*quad_degree;

		rho_f_p1 = _rho_f_p1;
		rho_f_p2 = _rho_f_p2;
		nu_f_p1 = _nu_f_p1;
		nu_f_p2 = _nu_f_p2;
		rho_s_p1 = _rho_s_p1;
		rho_s_p2 = _rho_s_p2;
		E_s_p1 = _E_s_p1;
		E_s_p2 = _E_s_p2;
		nu_s_p1 = _nu_s_p1;
		nu_s_p2 = _nu_s_p2;
	}

	virtual void pre_processing(vec2d_double& nodes, vec2d_double& weights) const
	{
		std::vector<double> nodes_1D, weights_1D;
		glq.quad_nodes_weights(quad_degree, weights_1D, nodes_1D);

		nodes.resize(2);
		for(int i = 0 ; i < 2 ; ++i)
		{
			nodes[i].resize(quad_degree_2D);
		}

		weights.resize(2);
		for(int i = 0 ; i < 2 ; ++i)
		{
			weights[i].resize(quad_degree_2D);
		}

		for(int i = 0 ; i < quad_degree ; ++i)		
		{
			for(int j = 0 ; j < quad_degree ; ++j)
			{
				nodes[0][i*quad_degree + j] = nodes_1D[i];
			}
		}
		for(int i = 0 ; i < quad_degree ; ++i)		
		{
			for(int j = 0 ; j < quad_degree ; ++j)
			{
				nodes[1][i*quad_degree + j] = nodes_1D[j];
			}
		}

		for(int i = 0 ; i < quad_degree ; ++i)		
		{
			for(int j = 0 ; j < quad_degree ; ++j)
			{
				weights[0][i*quad_degree + j] = weights_1D[i];
			}
		}
		for(int i = 0 ; i < quad_degree ; ++i)		
		{
			for(int j = 0 ; j < quad_degree ; ++j)
			{
				weights[1][i*quad_degree + j] = weights_1D[j];
			}
		}
	}	

	virtual std::vector<double> pre_processing() const
	{
		std::vector<double> nodes, weights, nodes_weights;

		glq.quad_nodes_weights(quad_degree, weights, nodes);

		for(int i = 0 ; i < quad_degree ; ++i)
		{
			nodes_weights.push_back(nodes[i]);
		}

		for(int i = 0 ; i < quad_degree ; ++i)
		{
			nodes_weights.push_back(weights[i]);
		}

		return nodes_weights;
	}

	virtual std::vector<double> pre_processing(const double& param1, const double& param2) const
	{
		// do nothing; useful only for MCS
		std::vector<double> dummy;

		return dummy;
	}

	virtual void simulation(std::vector<double>& pre_proc_result) const
	{
		
	}

	virtual void simulation(vec2d_double& nodes, vec2d_double& weights) const
	{
		std::string modify_nastin_data;
		std::string modify_solidz_data;
		int modify_nastin_data_ok = 0;
		int modify_solidz_data_ok = 0;
	
		double temp_nu_f = 0.0;
		double temp_rho_s = 0.0;

		std::string create_data_each_point;
		int run_create_data_point_ok = 0;
	
		for (int i = 0; i < quad_degree_2D; ++i)
		{
			create_data_each_point = run_create_data_point(create_data_point, i);
			run_create_data_point_ok = system(create_data_each_point.c_str());
			assert(run_create_data_point_ok >=0 );

			temp_nu_f = (0.5*nodes[0][i] + 0.5)*nu_f_p2 + nu_f_p1;
			assert(temp_nu_f >= 0);
			modify_nastin_data = run_insert_nastin_vis(insert_nastin_exec, nastin_dat, temp_nu_f, i);
			modify_nastin_data_ok = system(modify_nastin_data.c_str());
			assert(modify_nastin_data_ok >= 0);
			
			temp_rho_s = (0.5*nodes[1][i] + 0.5)*rho_s_p2 + rho_s_p1;
			assert(temp_rho_s >= 0);
			modify_solidz_data = run_insert_solidz_dens(insert_solidz_exec, solidz_dat, temp_rho_s, i);
			modify_solidz_data_ok = system(modify_solidz_data.c_str());
			assert(modify_solidz_data_ok >= 0);
		}
	}

	virtual void simulation(std::vector<double>& pre_proc_result_dim1, std::vector<double>& pre_proc_result_dim2) const
	{	
		/*TO DO : nothing here */
	}

	virtual void post_processing() const
	{
	
	}

	~SCSimulation_uniform() {}
};

#endif /* SCSIMUNIFORM_HPP_ */