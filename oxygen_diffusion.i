[GlobalParams]
  order = FIRST
  family = LAGRANGE
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 50
    ny = 1
    xmin = 0
    xmax = 0.57
    ymin = 0.0
    ymax = 0.2
    elem_type = QUAD4
  []
[]

[XFEM]
  qrule = volfrac
  output_cut_plane = true
[]

[UserObjects]
  [velocity]
    type = XFEMPhaseTransitionMovingInterfaceVelocityWithSymbol
    diffusivity_at_positive_level_set = 9.21e-5
    diffusivity_at_negative_level_set = 8.5e-6
    equilibrium_concentration_jump = 0.326
    value_at_interface_uo = value_uo
  []
  [value_uo]
    type = NodeValueAtXFEMInterface
    variable = 'u'
    interface_mesh_cut_userobject = 'cut_mesh'
    execute_on = TIMESTEP_END
    level_set_var = ls
  []
  [cut_mesh]
    type = InterfaceMeshCut2DUserObject
    mesh_file = generate_mesh_in.e
    interface_velocity_uo = velocity
    heal_always = true
    negative_id = 1
    positive_id = 33
  []
[]

[Constraints]
  [./xfem_constraint]
    type = XFEMEqualValueAtInterface
    geometric_cut_userobject = 'cut_mesh'
    use_displaced_mesh = false
    variable = u
    value = 0.324
    alpha = 100
  [../]
[]


[Variables]
  [u]
  []
[]

[ICs]
  [ic_u]
    type = FunctionIC
    variable = u
    function = 'if(x<0.565, 0.1126, 0.324)'
  []
[]

[AuxVariables]
  [ls]
    order = FIRST
    family = LAGRANGE
  []
[]


[Kernels]
  [diff]
    type = MatDiffusion
    variable = u
    diffusivity = diffusion_coefficient
  []
  [time]
    type = TimeDerivative
    variable = u
  []
[]

[AuxKernels]
  [ls]
    type = MeshCutLevelSetAux
    mesh_cut_user_object = cut_mesh
    variable = ls
    execute_on = 'TIMESTEP_BEGIN'
  []
[]

[Materials]
  [ZrO2]
    type = GenericConstantMaterial
    prop_names = A_diffusion_coefficient
    prop_values = 9.21e-5
  []
  [Zr_alpha]
    type = GenericConstantMaterial
    prop_names = B_diffusion_coefficient
    prop_values = 8.5e-6
  []
  [diff_combined]
    type = XFEMCutSwitchingMaterialReal
    cut_subdomain_ids = '1 33'
    base_names = 'B A'
    prop_name = diffusion_coefficient
    geometric_cut_userobject = cut_mesh
    outputs = 'exodus'
    output_properties = 'diffusion_coefficient'
  []
[]


[BCs]
  # Define boundary conditions
  [right_u]
    type = DirichletBC
    variable = u
    value = 0.344
    boundary = right
  []

  [left_u]
    type = NeumannBC
    variable = u
    boundary = left
    value = 0
  []
[]

[Executioner]
  type = Transient
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  line_search = 'none'

  l_tol = 1e-3
  nl_max_its = 15
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-8
  
  dt = 0.1
  num_steps = 5000  # 或更大
  max_xfem_update = 5
[]

[Outputs]
  execute_on = timestep_end
  exodus = true
[]