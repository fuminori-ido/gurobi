=begin
   This example reads a model from a file and tunes it.
   It then writes the best parameter settings to a file
   and solves the model using these parameters.
=end

describe 'Tune' do
  it 'works' do
    env = Gurobi::Env.new

    # Read model from file

    model = Gurobi::Model.new(env, model_file('p0033.mps'))

    menv = model.getEnv

    # Set the TuneResults parameter to 1

    menv.set_int(Gurobi::IntParam::TUNE_RESULTS, 1)

    # Tune the model

    model.tune

    # Get the number of tuning results

    resultcount = model.get_int(Gurobi::IntAttr::TUNE_RESULT_COUNT)

    if resultcount > 0

      # Load the tuned parameters into the model's environment

      model.getTuneResult(0)

      # Write tuned parameters to a file

      model.write("/tmp/tune.prm")

      # Solve the model using the tuned parameters

      model.optimize
    end
  end
end
