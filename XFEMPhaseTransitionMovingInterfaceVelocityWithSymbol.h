#pragma once

#include "XFEMMovingInterfaceVelocityBase.h"

class XFEMPhaseTransitionMovingInterfaceVelocityWithSymbol : public XFEMMovingInterfaceVelocityBase
{
public:
  static InputParameters validParams();

  XFEMPhaseTransitionMovingInterfaceVelocityWithSymbol(const InputParameters & parameters);
  virtual ~XFEMPhaseTransitionMovingInterfaceVelocityWithSymbol() {}

  virtual Real computeMovingInterfaceVelocity(dof_id_type point_id,
                                              RealVectorValue normal) const override;

protected:
  /// Diffusivity in the positive level set region
  Real _diffusivity_at_positive_level_set;

  /// Diffusivity in the negative level set region
  Real _diffusivity_at_negative_level_set;

  /// Jump of the equilibrium concentrations at phase boundary
  Real _equilibrium_concentration_jump;
};