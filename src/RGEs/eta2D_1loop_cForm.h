(3.*(eta1D.adjoint() * eta2D).trace() + (eta1L.adjoint() * eta2L).trace() + 3.*(eta2U.adjoint() * eta1U).trace())*eta1D + (3.*(eta2D.adjoint() * eta2D).trace() + (eta2L.adjoint() * eta2L).trace() + 3.*(eta2U.adjoint() * eta2U).trace() - (5.*pow(g1,2.))/12. - (9.*pow(g2,2.))/4. - 8.*pow(g3,2.))*eta2D + (eta1U*eta1U.adjoint()*eta2D)/2. - 2.*(eta1U*eta2U.adjoint()*eta1D) - (3.*(eta2U*eta2U.adjoint()*eta2D))/2. + (eta1D*eta1D.adjoint()*eta2D)/2. + (eta2D*eta1D.adjoint()*eta1D) + (3.*(eta2D*eta2D.adjoint()*eta2D))/2.