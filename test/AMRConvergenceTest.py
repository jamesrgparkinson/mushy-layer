# Function to run a convergence test
import os
from colorama import Fore
from MushyLayerRunSimple import MushyLayerRunSimple
from mushyLayerRunUtils import get_restart_file, get_executable_name,\
    get_final_chk_file, get_mushy_layer_dir
from BatchJob import BatchJob

def get_suffix(num):

    # return '-%d' % num
    return ''

def amr_convergence_test(params, full_output_dir, physicalProblem, nzs, 
                         num_procs = [1], num_restarts=0, analysis_command='',
                         restart_from_low_res=False):

    os.environ["CH_TIMER"] = "1"

    dependencies = []
    prev_job_id = -1
    prev_dir = ''
    prev_num_cells = 0

    while len(num_procs) < len(nzs):
        num_procs.append(num_procs[-1])

    param_i = -1

    for p in params:
        param_i = param_i + 1

        num_proc = p['num_proc']
                   
        # Don't repeat runs, unless we're restarting
        
        #cwd = os.getcwd()
        run_name = p['main.plot_prefix']
        test_name = run_name + get_suffix(0)
        full_path = os.path.join(full_output_dir, test_name)
        
        allow_restarts = False
        
        print(full_path)
        if os.path.exists(full_path):
        
            print(Fore.YELLOW + '    Run already done' + Fore.RESET)
            
            if num_restarts > 0:
            
                print('    ' + str(num_restarts) + ' restart(s) allowed \n ')
                # Find most recent folder and restart from there
                
                i = 0
                while os.path.exists(full_path):
                    i = i + 1
                    test_name = run_name + get_suffix(i)
                    full_path = os.path.join(full_output_dir, test_name)
                
                if i > num_restarts:
                    continue
                    
                most_recent_path = os.path.join(full_output_dir, run_name + get_suffix(i-1))
                # Now get the restart file
                restart_file = get_restart_file(most_recent_path)
                if restart_file:
                    p['main.restart_file'] = os.path.join(most_recent_path, restart_file)
                    allow_restarts = True
                    
                    print('    Set restart file: ' + p['main.restart_file'])
            else:
            
                continue
                
        else:
            print(Fore.YELLOW + '    No previous runs' + Fore.RESET)
      
        print(Fore.GREEN + '    **Do run**' + Fore.RESET)

        # Don't need output dir or exec dir here, MushyLayerRun will fill these in
        s = BatchJob('', p['concise_run_name'], '', num_proc)
        
        if restart_from_low_res:
            python_file = os.path.join(get_mushy_layer_dir(), 'test', 'create_refined_restart.py')
            new_dir = full_path
            
            if prev_job_id > -1:
                
                refinement = int(float(p['main.num_cells'][0])/float(prev_num_cells[0]))
                
                preprocess_cmd = 'python %s -p %s -n %s -r %d \n' % (python_file, prev_dir, new_dir, refinement)
                s.set_dependency(prev_job_id)
                s.set_preprocess(preprocess_cmd)
                p['main.restart_file'] = os.path.join(new_dir, 'restart.2d.hdf5')
            else:
                # Search for an old file we can restart from, just in case
                refinement = 2
                this_nx = int(p['main.num_cells'][0])
                coarse_nx = int(float(this_nx)/2.0)
                coarser_dir = new_dir.replace(str(this_nx), str(coarse_nx))
                
                if os.path.exists(coarser_dir) and get_final_chk_file(coarser_dir):
                    preprocess_cmd = 'python %s -p %s -n %s -r %d \n' % (python_file, coarser_dir, new_dir, refinement)
                    s.set_preprocess(preprocess_cmd)
                    p['main.restart_file'] = os.path.join(new_dir, 'restart.2d.hdf5')
                

        ml_run = MushyLayerRunSimple(full_output_dir, num_proc, p, s, allow_restarts, get_executable_name())
        ml_run.single_run(run_name)
        
        prev_job_id = s.job_id
        prev_dir = full_path
        prev_num_cells = p['main.num_cells']
        dependencies.append(s.job_id)

        print('==================')

        
    return dependencies




    
        

