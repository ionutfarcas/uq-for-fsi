#include "helper.hpp"
#include "MCsim_normal.hpp"
#include "MCsim_uniform.hpp"
#include "SCsim_normal.hpp"
#include "SCsim_uniform.hpp"

int main(int argc, char** argv)
{
    std::string config_file_name = "../configuration.uq";
    std::string nastin_dat;
    std::string solidz_dat;
    std::string run_exec;
    std::string output_data;
    std::string gather_data_exec_mc;
    std::string postproc_stat_exec_mc;
    std::string postproc_file_all_mc;
    std::string postproc_stat_mc;
    std::string gather_data_exec_sc;
    std::string get_output_sc;
    std::string postproc_stat_exec_sc;
    std::string output_file_sc;
    std::string coeff_sc;
    std::string postproc_stat_sc;
    std::string insert_nastin_exec;
    std::string insert_solidz_exec;
    std::string gather_alya_output;

    unsigned int uq_method = 0;
    unsigned int pdf = 0;
    unsigned int nsamples = 0;
    unsigned int ncoeff = 0;
    unsigned int prob_dim = 0;
    unsigned int sg_level = 0;
    double rho_f_p1 = 0.0;
    double rho_f_p2= 0.0;
    double nu_f_p1 = 0.0;
    double nu_f_p2 = 0.0;
    double rho_s_p1 = 0.0;
    double rho_s_p2 = 0.0;

    vec2d_float_t scs_pre_processing_results;
    std::vector<double> pre_proc_results_mcs_dim1;
    std::vector<double> pre_proc_results_mcs_dim2;

    auto t1 = std::chrono::high_resolution_clock::now();

    if(parse_configfile(
        config_file_name.c_str(), 
        nastin_dat, 
        solidz_dat, 
        run_exec, 
        output_data, 
        gather_data_exec_mc, 
        postproc_stat_exec_mc, 
        postproc_file_all_mc, 
        postproc_stat_mc, 
        gather_data_exec_sc,
        get_output_sc, 
        postproc_stat_exec_sc, 
        output_file_sc, 
        coeff_sc, 
        postproc_stat_sc, 
        insert_nastin_exec, 
        insert_solidz_exec,
        gather_alya_output, 
        uq_method, 
        pdf, 
        nsamples, 
        ncoeff, 
        prob_dim, 
        sg_level,
        rho_f_p1, 
        rho_f_p2, 
        nu_f_p1, 
        nu_f_p2, 
        rho_s_p1, 
        rho_s_p2) == 0)
    {
    	std::cout << "Error parsing the config file!" << std::endl;
        return 0;
    }

    if(uq_method == 0 && pdf == 0)
    {
        MCSimulation_normal mcs_n(
            nastin_dat, 
            solidz_dat, 
            run_exec, 
            output_data, 
            gather_data_exec_mc, 
            postproc_stat_exec_mc, 
            postproc_file_all_mc, 
            postproc_stat_mc, 
            insert_nastin_exec, 
            insert_solidz_exec,
            gather_alya_output, 
            nsamples, 
            rho_f_p1, 
            rho_f_p2, 
            nu_f_p1, 
            nu_f_p2, 
            rho_s_p1, 
            rho_s_p2);

        pre_proc_results_mcs_dim1 = mcs_n.pre_processing(nu_f_p1, nu_f_p2);
        pre_proc_results_mcs_dim2 = mcs_n.pre_processing(rho_s_p1, rho_s_p2);
        mcs_n.simulation(pre_proc_results_mcs_dim1, pre_proc_results_mcs_dim2);
        mcs_n.post_processing();
    }
    else if(uq_method == 0 && pdf == 1)
    {
        MCSimulation_uniform mcs_u(
            nastin_dat, 
            solidz_dat, 
            run_exec, 
            output_data, 
            gather_data_exec_mc, 
            postproc_stat_exec_mc, 
            postproc_file_all_mc, 
            postproc_stat_mc, 
            insert_nastin_exec, 
            insert_solidz_exec,
            gather_alya_output, 
            nsamples, rho_f_p1, 
            rho_f_p2, 
            nu_f_p1, 
            nu_f_p2, 
            rho_s_p1, 
            rho_s_p2);

        pre_proc_results_mcs_dim1 = mcs_u.pre_processing(nu_f_p1, nu_f_p2);
        pre_proc_results_mcs_dim2 = mcs_u.pre_processing(rho_s_p1, rho_s_p2);
        mcs_u.simulation(pre_proc_results_mcs_dim1, pre_proc_results_mcs_dim2);
        mcs_u.post_processing();   
    }
    else if(uq_method == 1 && pdf == 0)
    {
        SCSimulation_normal scs_n(
            nastin_dat, 
            solidz_dat, 
            run_exec, 
            output_data, 
            gather_data_exec_sc, 
            get_output_sc,
            postproc_stat_exec_sc, 
            output_file_sc, 
            coeff_sc,
            postproc_stat_sc, 
            insert_nastin_exec, 
            insert_solidz_exec,
            gather_alya_output, 
            ncoeff, 
            prob_dim,
            sg_level, 
            rho_f_p1, 
            rho_f_p2, 
            nu_f_p1, 
            nu_f_p2, 
            rho_s_p1, 
            rho_s_p2);
        scs_pre_processing_results = scs_n.pre_processing();
        scs_n.simulation(scs_pre_processing_results);
        scs_n.post_processing();
    }
    else if (uq_method == 1 && pdf == 1)
    {
         SCSimulation_uniform scs_u(
            nastin_dat, 
            solidz_dat, 
            run_exec, 
            output_data, 
            gather_data_exec_sc, 
            get_output_sc,
            postproc_stat_exec_sc, 
            output_file_sc, 
            coeff_sc,
            postproc_stat_sc, 
            insert_nastin_exec, 
            insert_solidz_exec,
            gather_alya_output, 
            ncoeff, 
            prob_dim,
            sg_level, 
            rho_f_p1, 
            rho_f_p2, 
            nu_f_p1, 
            nu_f_p2, 
            rho_s_p1, 
            rho_s_p2);

        scs_pre_processing_results = scs_u.pre_processing();
        scs_u.simulation(scs_pre_processing_results);
        scs_u.post_processing();
    }
    else
    {
        std::cout << "Unknown combitation; Please try again!" << std::endl;
        std::cout << "uq method: 0 -> monte carlo, 1 -> stochastic collocations, pdf: 0 -> normal, 1 -> uniform" << std::endl;
    }

    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "The simulation took "
    << std::chrono::duration_cast<std::chrono::seconds>(t2-t1).count()
    << " seconds\n";

    return 0;
}