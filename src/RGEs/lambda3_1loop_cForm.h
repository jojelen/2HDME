-3.*(eta1D.adjoint() * eta1D * eta2D.adjoint() * eta2D).trace() - 3.*(eta1D.adjoint() * eta1D * eta2U.adjoint() * eta2U).trace() + 3.*(eta1D.adjoint() * eta2D * eta1U.adjoint() * eta2U).trace() - 3.*(eta1D.adjoint() * eta2D * eta2D.adjoint() * eta1D).trace() + 3.*(eta1D.adjoint() * eta2U * eta1U.adjoint() * eta2D).trace() - 3.*(eta1D.adjoint() * eta2U * eta2U.adjoint() * eta1D).trace() - (eta1L.adjoint() * eta1L * eta2L.adjoint() * eta2L).trace() - (eta1L.adjoint() * eta2L * eta2L.adjoint() * eta1L).trace() - 3.*(eta1U.adjoint() * eta1U * eta2D.adjoint() * eta2D).trace() - 3.*(eta1U.adjoint() * eta1U * eta2U.adjoint() * eta2U).trace() + 3.*(eta1U.adjoint() * eta2D * eta1D.adjoint() * eta2U).trace() - 3.*(eta1U.adjoint() * eta2D * eta2D.adjoint() * eta1U).trace() + 3.*(eta1U.adjoint() * eta2U * eta1D.adjoint() * eta2D).trace() - 3.*(eta1U.adjoint() * eta2U * eta2U.adjoint() * eta1U).trace() - 3.*(eta2D.adjoint() * eta1D * eta1D.adjoint() * eta2D).trace() + 3.*(eta2D.adjoint() * eta1D * eta2U.adjoint() * eta1U).trace() - 3.*(eta2D.adjoint() * eta1U * eta1U.adjoint() * eta2D).trace() + 3.*(eta2D.adjoint() * eta1U * eta2U.adjoint() * eta1D).trace() - 3.*(eta2D.adjoint() * eta2D * eta1D.adjoint() * eta1D).trace() - 3.*(eta2D.adjoint() * eta2D * eta1U.adjoint() * eta1U).trace() - (eta2L.adjoint() * eta1L * eta1L.adjoint() * eta2L).trace() - (eta2L.adjoint() * eta2L * eta1L.adjoint() * eta1L).trace() - 3.*(eta2U.adjoint() * eta1D * eta1D.adjoint() * eta2U).trace() + 3.*(eta2U.adjoint() * eta1D * eta2D.adjoint() * eta1U).trace() - 3.*(eta2U.adjoint() * eta1U * eta1U.adjoint() * eta2U).trace() + 3.*(eta2U.adjoint() * eta1U * eta2D.adjoint() * eta1D).trace() - 3.*(eta2U.adjoint() * eta2U * eta1D.adjoint() * eta1D).trace() - 3.*(eta2U.adjoint() * eta2U * eta1U.adjoint() * eta1U).trace() + 2.*pow(abs(Lambda5),2.) + (3.*(pow(g1,4.) - 2.*pow(g1,2.)*(pow(g2,2.) + 2.*Lambda3) + 3.*(pow(g2,4.) - 4.*pow(g2,2.)*Lambda3)))/4. + 2.*(pow(Lambda4,2.) + Lambda3*(3.*(eta1D.adjoint() * eta1D).trace() + (eta1L.adjoint() * eta1L).trace() + 3.*(eta2D.adjoint() * eta2D).trace() + (eta2L.adjoint() * eta2L).trace() + 3.*((eta1U.adjoint() * eta1U).trace() + (eta2U.adjoint() * eta2U).trace() + Lambda1 + Lambda2) + 2.*Lambda3) + (Lambda1 + Lambda2)*Lambda4) + (3.*(eta1U.adjoint() * eta2U).trace() + 3.*(eta2D.adjoint() * eta1D).trace() + (eta2L.adjoint() * eta1L).trace())*(Lambda6 + Lambda7) + (3.*(eta1D.adjoint() * eta2D).trace() + (eta1L.adjoint() * eta2L).trace() + 3.*(eta2U.adjoint() * eta1U).trace() + 4.*Lambda6 + 8.*Lambda7)*conj(Lambda6) + (3.*(eta1D.adjoint() * eta2D).trace() + (eta1L.adjoint() * eta2L).trace() + 3.*(eta2U.adjoint() * eta1U).trace() + 8.*Lambda6 + 4.*Lambda7)*conj(Lambda7)