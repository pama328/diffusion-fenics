//
// Created by js on 13.05.16.
//

#ifndef DIFFUSION_FENICS_POISSONSOLVER_H
#define DIFFUSION_FENICS_POISSONSOLVER_H
#include <dolfin.h>

// Include generated headers for solving the Poisson-PDE
//#include "poisson1D.h"
//#include "poisson2D.h"
#include "poisson3D.h"

namespace Poisson {

// Wrapper class for changing Dimensions of Poissond-Problem
    template <int> class DimensionWrapper;

/*  //Provide FunctionSpace, Linear and Bilinear Form in 1D
  template <> class DimensionWrapper<1> {
  public:
      auto FunctionSpace(std::shared_ptr<dolfin::Mesh> mesh) ->
  poisson1D::FunctionSpace
      {return poisson1D::FunctionSpace(mesh);}

      auto LinearForm(std::shared_ptr<poisson1D::FunctionSpace> FunctionSpace)
  -> poisson1D::LinearForm
      { return poisson1D::LinearForm(FunctionSpace);}

      auto BilinearForm(std::shared_ptr<poisson1D::FunctionSpace>
  FunctionSpace1,
                        std::shared_ptr<poisson1D::FunctionSpace>
  FunctionSpace2) -> poisson1D::BilinearForm
      {return poisson1D::BilinearForm(FunctionSpace1, FunctionSpace2);}

  };

  //Provide FunctionSpace, Linear and Bilinear Form in 2D
  template <> class DimensionWrapper<2> {
  public:
      auto FunctionSpace(std::shared_ptr<dolfin::Mesh> mesh) ->
  poisson2D::FunctionSpace
      {return poisson2D::FunctionSpace(mesh);}

      auto LinearForm(std::shared_ptr<poisson2D::FunctionSpace> FunctionSpace)
  -> poisson2D::LinearForm
      { return poisson2D::LinearForm(FunctionSpace);}

      auto BilinearForm(std::shared_ptr<poisson2D::FunctionSpace>
  FunctionSpace1,
                        std::shared_ptr<poisson2D::FunctionSpace>
  FunctionSpace2) -> poisson2D::BilinearForm
      {return poisson2D::BilinearForm(FunctionSpace1, FunctionSpace2);}

  };

*/
// Provide FunctionSpace, Linear and Bilinear Form in 3D
    template <> class DimensionWrapper<3> {
    public:
        auto FunctionSpace(std::shared_ptr<dolfin::Mesh> mesh)
        -> poisson3D::FunctionSpace {
            return poisson3D::FunctionSpace(mesh);
        }

        auto LinearForm(std::shared_ptr<poisson3D::FunctionSpace> FunctionSpace)
        -> poisson3D::LinearForm {
            return poisson3D::LinearForm(FunctionSpace);
        }

        auto BilinearForm(std::shared_ptr<poisson3D::FunctionSpace> FunctionSpace1,
                          std::shared_ptr<poisson3D::FunctionSpace> FunctionSpace2)
        -> poisson3D::BilinearForm {
            return poisson3D::BilinearForm(FunctionSpace1, FunctionSpace2);
        }
    };

    template <const int dim, class SetupCase>
    void solvePDE(SetupCase& setup) {

        DimensionWrapper<dim> dimensionWrapper;

        // Setup FunctionSpace, Linear and BilinearForm (based on dim)
        auto V = std::make_shared<decltype(dimensionWrapper.FunctionSpace(setup.getMesh()))>(
                dimensionWrapper.FunctionSpace(setup.getMesh()));
        auto a = dimensionWrapper.BilinearForm(V, V);
        auto L = dimensionWrapper.LinearForm(V);

        // setup solution
        setup.setU(new dolfin::Function(V));



        auto dx = *setup.getSubDomainFunction();
        auto ds = *setup.getFacetFunction();

        a.dx = dx;
        a.ds = ds;
        L.g = *setup.getNeumann();
        L.f = *setup.getSource();
        L.dx = dx;
        L.ds = ds;

        // Define boundary condition
        dolfin::DirichletBC bc(*V,dolfin::Constant(5.0), ds,6); //ohne Boundary-Condition kein Segmentation-Fault
        // Compute solution
        dolfin::solve(a == L, *setup.getU(),bc);

        dolfin::File file("../output/poisson.pvd", "compressed");
        file << *setup.getU();

    }
}

#endif // DIFFUSION_FENICS_POISSONSOLVER_H