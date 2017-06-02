void FitDensityVsDepth()
{
	const Double_t MaxDepth = 150.0;
	const Double_t MinDepth = 0.0;
	const Double_t MaxDensity = 1.0;
	const Double_t MinDensity = 0.0;
	gStyle->SetOptStat(0);
	TCanvas *c = new TCanvas("c","c",1000,1000);
	TPad *p = new TPad("p","p",0,0,1,1);
	p->SetLeftMargin(0.15);
	p->SetRightMargin(0.15);
	p->SetGridx();
	p->SetGridy();
	p->Draw();
	p->cd();
	TH2F *h2 = new TH2F("h2","",10,-MaxDepth,MinDepth,10,MinDensity,MaxDensity);
	h2->GetXaxis()->SetTitle("Depth (m)");
	h2->GetXaxis()->SetTitleOffset(1.5);
	h2->GetYaxis()->SetTitleOffset(1.5);
	h2->GetYaxis()->SetTitle("Density (g cm^{-3})");
	h2->Draw("");
	gPad->SetFixedAspectRatio();
	
	TGraph *load1 = new TGraph("ARIANNA_BoreHole_1.dat");
	Double_t *x1 = load1->GetX();
	Double_t *y1 = load1->GetY();
	for(Int_t i=0;i<load1->GetN();++i)
	{
		x1[i] = -x1[i];
		y1[i] = y1[i]/1000.0;
	}
	TGraph *Moores_Bay_Hole_1 = new TGraph(load1->GetN(),x1,y1);
	delete load1;
	TGraph *load2 = new TGraph("ARIANNA_BoreHole_2.dat");
	Double_t *x2 = load2->GetX();
	Double_t *y2 = load2->GetY();
	for(Int_t i=0;i<load2->GetN();++i)
	{
		x2[i] = -x2[i];
		y2[i] = y2[i]/1000.0;
	}
	TGraph *Moores_Bay_Hole_2 = new TGraph(load2->GetN(),x2,y2);
	delete load2;
	
	TF1 *Hole_1_Fit = new TF1("Hole_1_Fit","0.9167-[0]*exp([1]*x)",-MaxDepth,MinDepth);
	TF1 *Hole_2_Fit = new TF1("Hole_2_Fit","0.9167-[0]*exp([1]*x)",-MaxDepth,MinDepth);
	TF1 *Asymp = new TF1("Asymp","0.9167",-MaxDepth,MinDepth);
	Moores_Bay_Hole_1->Fit("Hole_1_Fit","R0");
	Moores_Bay_Hole_2->Fit("Hole_2_Fit","R0");
	
	char result1[100];
	sprintf(result1,"A-B exp(Cz), B: %2.2f +/- %1.2f, C: %2.3f +/- %1.3f",
		Hole_1_Fit->GetParameter(0),Hole_1_Fit->GetParError(0),Hole_1_Fit->GetParameter(1),Hole_1_Fit->GetParError(1));
	char result2[100];
	sprintf(result2,"A-B exp(Cz), B: %2.3f +/- %1.3f, C: %2.3f +/- %1.3f",
		Hole_2_Fit->GetParameter(0),Hole_2_Fit->GetParError(0),Hole_2_Fit->GetParameter(1),Hole_2_Fit->GetParError(1));
	
	Moores_Bay_Hole_1->SetMarkerStyle(2);
	Moores_Bay_Hole_1->SetMarkerSize(2);
	Moores_Bay_Hole_1->SetMarkerColor(kBlack);
	Moores_Bay_Hole_1->GetYaxis()->SetRangeUser(MinDensity,MaxDensity);
	Moores_Bay_Hole_1->GetXaxis()->SetRangeUser(-MaxDepth,MinDepth);
	Moores_Bay_Hole_2->SetMarkerStyle(4);
	Moores_Bay_Hole_2->SetMarkerSize(2);
	Moores_Bay_Hole_2->SetMarkerColor(kBlack);
	Moores_Bay_Hole_2->GetYaxis()->SetRangeUser(MinDensity,MaxDensity);
	Moores_Bay_Hole_2->GetXaxis()->SetRangeUser(-MaxDepth,MinDepth);
	Hole_1_Fit->SetLineColor(kBlack);
	Hole_2_Fit->SetLineColor(kBlack);
	Hole_1_Fit->SetLineWidth(2);
	Hole_2_Fit->SetLineWidth(2);
	Hole_1_Fit->SetLineStyle(3);
	Hole_2_Fit->SetLineStyle(7);
	Asymp->SetLineColor(kBlack);
	Asymp->SetLineWidth(2);
	Asymp->SetLineStyle(1);
	
	Moores_Bay_Hole_1->Draw("P same");
	Moores_Bay_Hole_2->Draw("P same");
	Hole_1_Fit->Draw("same");
	Hole_2_Fit->Draw("same");
	Asymp->Draw("same");
	
	TLegend *legend = new TLegend(0.15,0.125,0.85,0.35);
	legend->AddEntry(Moores_Bay_Hole_1,"Moore's Bay Borehole #1","p");
	legend->AddEntry(Hole_1_Fit,result1,"l");
	legend->AddEntry(Moores_Bay_Hole_2,"Moore's Bay Borehole #2","p");
	legend->AddEntry(Hole_2_Fit,result2,"l");
	legend->AddEntry(Asymp,"Bulk ice value (#rho = 0.9167)","l");
	legend->Draw("same");
	
	char fileOut[100];
	sprintf(fileOut,"January10_plot1.pdf");
	c->Print(fileOut);
}
