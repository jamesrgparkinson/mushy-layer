/*
 * mushyLayerOpt.h
 *
 *  Created on: 11 Feb 2019
 *      Author: parkinsonjl
 *
 *      This structure contains all the options for the mushy layer code which shouldn't change
 *      during a simulation
 */

#ifndef SRC_MUSHYLAYEROPT_H_
#define SRC_MUSHYLAYEROPT_H_

// Comment below ensures that enums doxygen generates documentation for enums declared in this file
/*!
@file
Contains immunity flag definitions
*/

/// Different multigrid methods
enum MGmethod {
  /// Normal linear multigrid
  MGTypeStandard,

  /// Full Approximation Scheme (for nonlinear problems)
  MGTypeFAS
};

enum TaggingMethod {
  /// Tag where the undivided gradient of some field is bigger than some value
  UndividedGradient,

  /// Tag where the magnitude of some field is bigger than some values
  Magnitude,

  /// Tag where the value of some field is larger than some criteria
  CompareLargerThan,

  /// Tag where the value of some field multiplied by -1 is larger than some criteria.
  /**
   * Alternatively, can think of this as when some field is smaller than some criteria.
   */
  CompareLargerThanNegative
};

enum RefluxMethod {
  /// 0 - linear correction
  LinearReflux,

  /// 1 - linear VC correction
  LinearVCReflux,

  /// 2 - nonlinear correction
  NonlinearReflux
};

/// Options for handling advection velocities, where S is the source term
enum velocityAdvectionTypes
{
  /// solve \f$ \frac{\partial (\mathbf{u}/\chi)}{\partial t} + (u/porosity) \cdot \nabla (u/porosity) = S/porosity (-(u/porosity^2)dporosity/dt) \f$
  m_porosityInAdvection,

  /// solve du/dt + (u/porosity).grad(u) = S + (u.grad(porosity)/porosity^2)u
  m_porosityOutsideAdvection,

  /// solve du/dt + u.grad(u) = S
  m_noPorosity

};


/// Different options for enforcing a fixed porosity
enum PorosityFunctions
{
  ///  Porosity = 1.0 everywhere
  constantLiquid,

  /// Porosity varies linearly with x, \f$ \chi = 0.1 + 0.9 x \f$
  linear,

  /// Porosity looks like a gaussian, \f$ \chi =  0.1 + 0.9 exp(-100 (x-0.5)^2) \f$
  gaussian,

  /// Gaussian with some sinuosoidal variation, \f$ \chi = (exp(-(x-0.5)^2))*(0.5 + 0.1 sin(2 \pi y)) \f$
  gaussianSinusoidal,

  /// Porosity = 0.1
  constantSmall,

  /// Porosity varies with the y-direction cubed, \f$ \chi = 0.01 + (y-0.01)^3 \f$
  cubic,

  /// \f$ \chi = 0.5 ( (1-tanh(20 y)) + (1-x)) \f$
  hyperbolicTan,
};

/// Identifiers for vector variables
  enum VectorVars {
    m_fluidVel,
    m_U_porosity,
    m_Ustar,
    m_advUstar,
    m_advectionVel,
    m_viscousSolveSrc,
    m_UdelU,
    m_advectionSrc,
    m_fluidVelAnalytic,
    m_fluidVelErr,
    m_dUdt,
    m_FsDiffusion,
    m_FsFluid,
    m_Fs,
    m_freestreamCorrection,
    m_UpreProjection,
    m_advectionImplicitSrc,
    m_MACcorrection,
    CCcorrection,
    m_advSrcLapU,
    m_advUpreProjection,
    m_bodyForce,
    m_advVelCorr,

    /// Number of vector variables
    // Make sure this comes last!
    m_numVectorVars
  };

  /// Identifiers for different scalar variables
  enum ScalarVars {
    m_enthalpy,
    m_bulkConcentration,
    m_temperature,
    m_porosity,
    m_liquidConcentration,
    m_solidConcentration,
    m_pressure,
    m_permeability,
    m_viscosity,
    m_lambda,
    m_lambda_porosity,
    m_enthalpySolidus,
    m_enthalpyLiquidus,
    m_enthalpyEutectic,
    m_porosityAnalytic,
    m_temperatureAnalytic,
    m_saltEqnSrcGodunov,
    m_saltEqnSrcFiniteDiff,
    m_saltEqnOp,
    m_Terr,
    m_enthalpyOp,
    m_enthalpySrc,
    m_divUadv,
    m_dHdt,
    m_dSdt,
    m_averageVerticalFlux,
    m_soluteFluxAnalytic,
    m_verticalFlux,
    m_saltResidual,
    m_divU,
    m_averageHeatFlux,
    m_streamfunction,
    m_vorticity,
    m_FsVertDiffusion,
    m_FsVertFluid,
    m_FsVertFrame,
    m_divUcorr,
    m_pi,
    m_phi,
    m_MACBC,
    m_MACrhs,
    m_CCrhs,

    /// Number of scalars variables
    // Make sure this comes last!
    m_numScalarVars

  };


//TODO: these should probably all be const's
//TODO: add comments to these options so it's obvious what they do
//TODO: collate more options in here rather than having lots of parmparses in the main code, so it's obvious what all the options are
/// Contains most of the options for running the code, and how to handle the equations
struct MushyLayerOptions {

    /// Output directory for plot and checkpoint files
  string output_dir;

  /// Prefix for plot files
  string plotfile_prefix;

  /// Turn on to only produce a minimal ammount of output
  bool minimalOutput;

  /// Turn on to produce lots of output
  bool debug;

  /// Maximum value by which the solution may have changed for us to have reached steady state
  Real steadyStateCondition;

  /// Turn on to ignore changes in velocity when deciding if we've reached steady state
  bool ignoreVelocitySteadyState;

  /// Turn on to ignore changes in bulk concentration when deciding if we've reached steady state
  bool ignoreBulkConcSteadyState;

  /// Domain width
  Real domainWidth;

  /// Domain height
  Real domainHeight;

  /// Maximum allowed CFL number
  Real cfl;

  /// TODO: remove this
  bool skipUnsafeCFL;

  /// Force the code to compute a CFL condition based on the size of \f$\mathbf{U}/\chi\f$, rather than \f$ \mathbf{U} \f$
  bool forceUseUChiForCFL;

  /// Minimum time to run simulations for (even if the steady state condition has been met)
  Real min_time;

  /// Maximum allowed fractional increase in dt from one timestep to the next
  Real max_dt_growth;

  /// Multiply the dt computed during initialisation procedures by this factor before using it
  /**
   * Useful for using a smaller dt initially for stability
   * TODO: can probably remove this, it's just a duplicate of initial_dt_multiplier really
   */
  Real init_dt_scale;

  /// Enforce a fixed timestep  (if > 0)
  Real fixedDt;

  /// Scale initial dt's by this factor
  Real initial_dt_multiplier;

  /// Minimum dt allowed
  Real minDt;

  /// Maximum dt allowed
  Real max_dt;

  /// Maximum initial dt allowed
  Real max_init_dt;

  /// CFL number for acceleration. Only used if useAccelDt = true
  /**
   * When velocities are changing rapidly due to a large acceleration, the timestep
   * computed based on the current velocities may not be a appropriate. Instead,
   * we should estimate the new velocities given the acceleration:
   *
   * \f$ \Delta t = \sqrt{accelCFL \Delta x / acceleration} \f$
   *
   * In that case, we use this CFl number.
   */
  Real accelCFL;

  /// Turn on to print some diagnostics about the dt computed according to acceleration considerations
  bool printAccelDt;

  /// Use the timesteps computed according to the cfl condition applied to accelerations
  bool useAccelDt;

  /// For the first timestep, use dt computed according to the cfl condition applied to accelerations
  bool useInitAccelDt;

  /// Maximum level allowed in this AMR hierarchy
  int max_possible_level;

  /// Turn on to compute the vorticity/streamfunction as a diagnostic
  bool computeVorticityStreamFunction;

  /// Turn on to use fortran routines for regularising the solution on cell faces (i.e. ensuring porosity is not 0)
  /**
   * Turned off by default as doesn't work yet.
   */
  bool useFortranRegularisationFace;

  /// Turn on using fortran routines for regularising the solution on cell centres (i.e. ensuring porosity is not 0)
  bool useFortranRegularisation;


//  Real stokesDarcyForcingTimescale;

  bool viscousBCs;

  int num_init_passes;
  Real restart_new_time;
  bool init_add_subtract_grad_p;
  bool init_compute_uDelu;
  bool increaseDt;

  Real spongeHeight;
  Real postTraceSmoothing;
  bool skipNewLevelScalars;
  bool skipSaltUpdate;
  bool skipHCUpdate;
  bool doDiffusionSrc;

  bool noMultigrid;
  int noMultigridIter;

  Real rampBuoyancy;
  Real initRaC;
  Real initRaT;
  Real maxRaC;
  Real maxRaT;

  Real advVelCentering;
  Real adv_vel_centering_growth;
  int solverFailRestartMethod;
  bool ignoreSolveFails;
  bool initiallyDoAutomaticRestart;
  int steadyStateNormType;
  bool load_advVel;
  Real CFinterpOrder_advection;


  bool nonlinearHCOpSuperOptimised;

  /// Whether or not to do subcycling
  bool useSubcycling;
  int verbosity;
  /// Use slope limiting in advection calculations?
  bool useLimiting;

  /// Tag buffer size
  int tagBufferSize;
  /// Refinement threshold
  Real refineThresh;

  bool doEulerPart;

  bool computeDiagnostics;

  // Projection stuff
  bool doProjection;
  bool useIncrementalPressure;
  Real phiScale;
  bool scaleMACBCWithChi;
  Real MACBCscale;

  bool doSyncOperations;
  bool enforceAnalyticSoln;
  bool useAnalyticSource;
  int analyticSolution;

  Real maxDivUFace;
  bool scaleP_MAC;
  bool scaleP_CC;
  bool explicitDarcyTerm;
  bool usePiAdvectionBCs;
  int projection_verbosity;

  bool implicitAdvectionSolve;
  bool usePhiForImplicitAdvectionSolve;

  bool multiCompUStarSolve;

  Real solidPorosity;
  Real advPorosityLimit;
  Real chiLimit;
  Real ccVelPorosityLimit;
  Real advVelsrcChiLimit;
  Real uDelU_porosityLimit;
  Real advVelChiLimit;

  bool useOldAdvVel;
  bool enforceAnalyticVel;
  bool projectAnalyticVel;
  int analyticVelType;
  bool initAnalyticVel;

  int lapVelNumSmooth;
  Real lapVelSmoothScale;

  int maxProjBaseLevel;
  int maxNumMACProj;

  int lapVelBCOrder;
  Real CCVelSrcTermCentering;
  bool legacyComputePredictVel;

  // u.grad(u) stuff
  int uDeluMethod;
  int uDelU_grow;
  bool uDelUConservativeForm;

  // Advection source term
  bool advVelPressureSrc;
  bool advVelDarcySrc;
  bool advVelViscousSrc;
  bool advVelBuoyancySrc;
  bool advSrcAllowLaggedLapVel;

  bool CCAdvSrc;
  bool CCDarcySrc;
  bool CCBuoyancySrc;
  bool CCPressureSrc;
  bool CCPressureSrcOverride;

  bool do_postRegrid_smoothing;
  bool reflux_momentum;
  bool reflux_normal_momentum;

  bool reflux_enthalpy ;
  bool reflux_concentration ;
  bool reflux_lambda ;

  bool refluxAverageDown;
  RefluxMethod refluxMethod;
  Real refluxBetaSign;
  Real refluxCorrSign;

  Real viscous_solver_tol;
  int viscous_num_smooth_down;
  int viscous_num_smooth_up;

  int AMRMultigridRelaxMode;
  int AMRMultigridVerb;
  Real AMRMultigridTolerance;
  Real AMRMultigridHang;
  Real AMRMultigridNormThresh;

  int velMGNumSmooth;
  Real velMGTolerance;
  Real velMGHang;
  int velMGNumMG;
  Real velMGNormThresh;
  int VelMGMaxIter;

  int HCMultigridNumSmoothUp;
  int HCMultigridNumSmoothDown;
  int HCMultigridNumMG;
  int HCMultigridMaxIter;
  int HCMultigridVerbosity;
  int HCMultigridBottomSolveIterations;
  Real HCMultigridTolerance;
  Real HCMultigridHang;
  Real HCMultigridNormThresh;
  int HCMultigridRelaxMode; // 1=GSRB, 4=jacobi
  bool HCMultigridUseRelaxBottomSolverForHC;

  int velAdvNormalPredOrder;
  bool velAdvUseFourthOrderSlopes;
  bool velAdvHigherOrderLimiter;
  bool velAdvUseArtVisc;
  Real velAdvArtVisc;
  bool HCUseArtVisc;
  Real HCArtVisc;
  int HCNormalPredOrder;
  bool HCUseFourthOrderSlopes;
  bool HCHigherOrderLimiter;

  int taggingVar;
  int taggingVectorVar;
  TaggingMethod taggingMethod;
  Real min_regrid_time;
  Real fixed_grid_time;
  bool tagDomainBoundary;
  bool tagMLboundary;
  bool tag_velocity;
  Real vel_thresh;
  bool tag_plume_mush;
  Real plumeVelThreshold;
  Real plumeSalinityThreshold;
  Real taggingMarginalPorosityLimit;
  Real regridTime;
  // Tag cells in the centre of the domain to make a box of this size. Leave as 0 (default) to do nothing
  int tagCenterBoxSize;
  bool testRegridCoarsening;

  bool scalarHOinterp;
  bool vectorHOinterp;

  bool makeRegridPlots;
  Real regrid_dt_scale;

  bool initLambda;
  Real variable_eta_factor;
  Real minEta;
  bool computeFreestreamCorrectionSingleLevel;
  bool regrid_advect_before_freestream;
  bool regrid_freestream_subcycle;
  Real regrid_eta_scale;

  /// default = 0: no smoothing
  Real regrid_smoothing_coeff;

  bool project_initial_vel;
  bool initialize_pressures;
  bool addSubtractGradP;

  int iter_plot_interval;

  int customInitData;
  bool writePressureInitFields;
  bool initResetStates;

  Real skipTrickySourceTerm;
  bool allowMulticompAdvection;
  Real smoothingCoeff;

  bool compute_initial_VD_corr;

  int timeIntegrationOrder;
  int AMRMultigrid_verbosity;

  Real lowerPorosityLimit;
  Real initialPerturbation;

  bool doScalarAdvectionDiffusion;
  Real initVel;

  Real perturbationPhaseShift;
  Real delayedPerturbation = 0.0;
  Real perturbationTime = 0.0;
  Real perturbationWavenumber = 1.0;
  bool perturbationSin = false;
  Real fixedPorosity = -1.0;
  Real porosityTimescale;
  PorosityFunctions porosityFunction;

  int maxRestartWavenumbers;
  Real restartPerturbation;
  Real radius;
  Real initVelScale;

  bool linearGradient;
  Real mushHeight;
  int summerProfile;
  Real meltPondDepth;
  bool horizAverageRestart;
  int restart_perturbation_var;



  // Restart
  Real refTemp;
  Real refSalinity;
  Real prevRefSalinity;
  Real prevRefTemp;
  Real dtReductionFactor;

  // Porosity
  Real fixedPorosityMaxChi;
  Real FixedPorositySTD;
  Real fixedPorosityFractionalInnerRadius;
  Real fixedPorosityEndTime;

  bool scalarExchangeCorners;
  Real buoyancy_zero_time;

  /// If this is set to greater than 0,
  Real maxEta;

  /// Whether to use linear or FAS geometric multigrid
  /**
   * Should really be FAS as we're solving nonlinear problems, but leaving the option
   * here to change back to linear multigrid.
   */
  MGmethod MGtype;

  /// Specify how to treat the \f$ \mathbf{U} \cdot \nabla \left( \mathbf{U}/\chi \right) \f$ term
  velocityAdvectionTypes advectionMethod;

};


#endif /* SRC_MUSHYLAYEROPT_H_ */
