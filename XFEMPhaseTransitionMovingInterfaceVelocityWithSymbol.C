#include "XFEMPhaseTransitionMovingInterfaceVelocityWithSymbol.h"

registerMooseObject("WorkerApp", XFEMPhaseTransitionMovingInterfaceVelocityWithSymbol);

InputParameters
XFEMPhaseTransitionMovingInterfaceVelocityWithSymbol::validParams()
{
  InputParameters params = XFEMMovingInterfaceVelocityBase::validParams();
  params.addRequiredParam<Real>("diffusivity_at_positive_level_set",
                                "Diffusivity for level set positive region.");
  params.addRequiredParam<Real>("diffusivity_at_negative_level_set",
                                "Diffusivity for level set negative region.");
  params.addRequiredParam<Real>("equilibrium_concentration_jump",
                                "The jump of the equilibrium concentration at the interface.");
  params.addClassDescription(
      "calculate the interface velocity for a simple phase transition problem.");
  return params;
}

XFEMPhaseTransitionMovingInterfaceVelocityWithSymbol::XFEMPhaseTransitionMovingInterfaceVelocityWithSymbol(
    const InputParameters & parameters)
  : XFEMMovingInterfaceVelocityBase(parameters),
    _diffusivity_at_positive_level_set(getParam<Real>("diffusivity_at_positive_level_set")),
    _diffusivity_at_negative_level_set(getParam<Real>("diffusivity_at_negative_level_set")),
    _equilibrium_concentration_jump(getParam<Real>("equilibrium_concentration_jump"))
{
}

Real
XFEMPhaseTransitionMovingInterfaceVelocityWithSymbol::computeMovingInterfaceVelocity(
    dof_id_type point_id, RealVectorValue normal) const
{
  Real value_positive = _value_at_interface_uo->getValueAtPositiveLevelSet()[point_id];
  Real value_negative = _value_at_interface_uo->getValueAtNegativeLevelSet()[point_id];
  RealVectorValue grad_positive = _value_at_interface_uo->getGradientAtPositiveLevelSet()[point_id];
  RealVectorValue grad_negative = _value_at_interface_uo->getGradientAtNegativeLevelSet()[point_id];

  /*Moose::out << "Interface velocity: " << ((-_diffusivity_at_positive_level_set * grad_positive) -
                   (-_diffusivity_at_negative_level_set * grad_negative)) *
                  normal / (value_positive - value_negative + _equilibrium_concentration_jump) << std::endl;
  
  Moose::out << "grad_positive: " << grad_positive << std::endl;
  Moose::out << "grad_negative: " << grad_negative << std::endl;
  Moose::out << "value_positive: " << value_positive << std::endl;
  Moose::out << "value_negative: " << value_negative << std::endl;
  Moose::out << "(-_diffusivity_at_positive_level_set * grad_positive): " << (-_diffusivity_at_positive_level_set * grad_positive) << std::endl;
  Moose::out << "(-_diffusivity_at_negative_level_set * grad_negative): " << (-_diffusivity_at_negative_level_set * grad_negative) << std::endl;
  Moose::out << "(value_positive - value_negative + _equilibrium_concentration_jump): " << (value_positive - value_negative + _equilibrium_concentration_jump) << std::endl;*/

  return ((-_diffusivity_at_positive_level_set * grad_positive) -
                   (-_diffusivity_at_negative_level_set * grad_negative)) *
                  normal / (value_positive - value_negative + _equilibrium_concentration_jump);
}