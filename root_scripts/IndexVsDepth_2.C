void IndexVsDepth_2()
{
	const Double_t MaxDepth = 150.0;
	const Double_t MinDepth = 0.0;
	const Double_t MaxIndex = 1.9;
	const Double_t MinIndex = 1.0;
	const Double_t MinResidualIndex = -0.1;
	const Double_t MaxResidualIndex = 0.1;
	Float_t scalefactor = 3.0;
	Float_t globalMarkerSize = 1.75;
	Float_t globalLineWidth = 2.5;

	gStyle->SetOptStat(0);
	TCanvas *c = new TCanvas("c","c",1000,1000);
	TPad *p = new TPad("p","p",0,0,1,0.75);
	TPad *p2 = new TPad("p2","p2",0,0.75,1,1);
	p->SetLeftMargin(0.15);
	p2->SetLeftMargin(0.15);
	p->SetRightMargin(0.15);
	p2->SetRightMargin(0.15);
	p->SetBottomMargin(0.12);
	p2->SetBottomMargin(0.15);
	p->SetGridx();
	p2->SetGridx();
	p->SetGridy();
	p2->SetGridy();
	p->Draw();
	p2->Draw();
	p->cd();
	TH2F *h2 = new TH2F("h2","",10,-MaxDepth,MinDepth,10,MinIndex,MaxIndex);
	h2->GetYaxis()->SetNdivisions(5,kTRUE);
	h2->GetXaxis()->SetTitle("Depth (m)");
	h2->GetXaxis()->SetTitleSize(0.04);
	h2->GetXaxis()->SetTitleOffset(1.3);
	h2->GetXaxis()->SetLabelSize(0.04);
	h2->GetYaxis()->SetTitleOffset(1.0);
	h2->GetYaxis()->SetLabelSize(0.04);
	h2->GetYaxis()->SetTitle("n(z)");
	h2->GetYaxis()->SetTitleSize(0.05);
	h2->Draw("");
	gPad->SetFixedAspectRatio();
	p2->cd();
	TH2F *h3 = new TH2F("h3","",10,-MaxDepth,MinDepth,10,MinResidualIndex,MaxResidualIndex);
	h3->GetXaxis()->SetTitle("Depth (m)");
	h3->GetXaxis()->SetTitleSize(0.04);
	h3->GetXaxis()->SetTitleOffset(1.3*scalefactor);
	h3->GetXaxis()->SetLabelSize(0.04*scalefactor);
	h3->GetYaxis()->SetTitleOffset(0);
	h3->GetYaxis()->SetLabelSize(0.04*scalefactor);
	h3->GetYaxis()->SetTitle("n(z) - Fit");
	h3->GetYaxis()->SetTitleSize(0.04*scalefactor);
	h3->GetYaxis()->SetNdivisions(5,kTRUE);
	h3->Draw("");
	gPad->SetFixedAspectRatio();
	p->cd();
	
	TGraph *load1 = new TGraph("../IceData/ARIANNA_BoreHole_1.dat");
	Double_t *x1 = load1->GetX();
	Double_t *y1 = load1->GetY();
	for(Int_t i=0;i<load1->GetN();++i)
	{
		x1[i] = -x1[i];
		y1[i] = 1.0+0.86*y1[i]/1000.0;
	}
	TGraph *Moores_Bay_Hole_1 = new TGraph(load1->GetN(),x1,y1);
	delete load1;
	
	TGraph *load2 = new TGraph("../IceData/ARIANNA_BoreHole_2.dat");
	Double_t *x2 = load2->GetX();
	Double_t *y2 = load2->GetY();
	for(Int_t i=0;i<load2->GetN();++i)
	{
		x2[i] = -x2[i];
		y2[i] = 1.0+0.86*y2[i]/1000.0;
	}
	TGraph *Moores_Bay_Hole_2 = new TGraph(load2->GetN(),x2,y2);
	delete load2;
	
	TGraph *load3 = new TGraph("../IceData/SPICE_data.dat");
	Double_t *x3 = load3->GetX();
	Double_t *y3 = load3->GetY();
	for(Int_t i=0;i<load3->GetN();++i)
	{
		x3[i] = -x3[i];
		y3[i] = 1.0+0.86*y3[i]/1000.0;
	}
	TGraph *SPICE_Hole = new TGraph(load3->GetN(),x3,y3);
	delete load3;
	
	TGraph *RICE_Data = new TGraph("../IceData/RICE_data.csv");
	TGraph *Byrd_Data = new TGraph("../IceData/Byrd_data.csv");
	TGraph *Mizuho_Data = new TGraph("../IceData/Mizuho_data.csv");
	
	TF1 *Hole_1_Fit = new TF1("Hole_1_Fit","1.78-[0]*exp([1]*x)",-MaxDepth,MinDepth);
	TF1 *Hole_2_Fit = new TF1("Hole_2_Fit","1.78-[0]*exp([1]*x)",-MaxDepth,MinDepth);
	TF1 *SPICE_Fit = new TF1("SPICE_Fit","1.78-[0]*exp([1]*x)",-MaxDepth,MinDepth);
	SPICE_Fit->SetParameter(0,0.3);
	SPICE_Fit->SetParameter(1,0.02);
	TF1 *RICE_Data_Fit = new TF1("RICE_Data_Fit","1.78-[0]*exp([1]*x)",-MaxDepth,MinDepth);
	TF1 *Byrd_Data_Fit = new TF1("Byrd_Data_Fit","1.78-[0]*exp([1]*x)",-MaxDepth,MinDepth);
	TF1 *Mizuho_Data_Fit = new TF1("Mizuho_Data_Fit","1.78-[0]*exp([1]*x)",-MaxDepth,MinDepth);;
	Moores_Bay_Hole_1->Fit("Hole_1_Fit","R0");
	Moores_Bay_Hole_2->Fit("Hole_2_Fit","R0");
	RICE_Data->Fit("RICE_Data_Fit","R0");
	Byrd_Data->Fit("Byrd_Data_Fit","R0");
	Mizuho_Data->Fit("Mizuho_Data_Fit","R0");
	SPICE_Hole->Fit("SPICE_Fit","R0");
	
	char result1[100];
	sprintf(result1,"A-B exp(Cz), B: %2.2f +/- %1.2f, C: %2.3f +/- %1.3f",
		Hole_1_Fit->GetParameter(0),Hole_1_Fit->GetParError(0),Hole_1_Fit->GetParameter(1),Hole_1_Fit->GetParError(1));
	char result2[100];
	sprintf(result2,"A-B exp(Cz), B: %2.3f +/- %1.3f, C: %2.3f +/- %1.3f",
		Hole_2_Fit->GetParameter(0),Hole_2_Fit->GetParError(0),Hole_2_Fit->GetParameter(1),Hole_2_Fit->GetParError(1));
	char result3[100];
	sprintf(result3,"A-B exp(Cz), B: %2.3f +/- %1.3f, C: %2.3f +/- %1.3f",
		RICE_Data_Fit->GetParameter(0),RICE_Data_Fit->GetParError(0),RICE_Data_Fit->GetParameter(1),RICE_Data_Fit->GetParError(1));
	char result4[100];
	char result6[100];
	sprintf(result6,"A-B exp(Cz), B: %2.3f +/- %1.3f, C: %2.4f +/- %1.4f",
		Byrd_Data_Fit->GetParameter(0),Byrd_Data_Fit->GetParError(0),Byrd_Data_Fit->GetParameter(1),Byrd_Data_Fit->GetParError(1));
	char result7[100];
	sprintf(result7,"A-B exp(Cz), B: %2.3f +/- %1.3f, C: %2.3f +/- %1.3f",
		Mizuho_Data_Fit->GetParameter(0),Mizuho_Data_Fit->GetParError(0),Mizuho_Data_Fit->GetParameter(1),Mizuho_Data_Fit->GetParError(1));
	char result8[100];
	sprintf(result8,"A-B exp(Cz), B: %2.3f +/- %1.3f, C: %2.3f +/- %1.4f",
		SPICE_Fit->GetParameter(0),SPICE_Fit->GetParError(0),SPICE_Fit->GetParameter(1),SPICE_Fit->GetParError(1));
	
	Moores_Bay_Hole_1->SetMarkerStyle(24);
	Moores_Bay_Hole_2->SetMarkerStyle(25);
	RICE_Data->SetMarkerStyle(26);
	Byrd_Data->SetMarkerStyle(5);
	Mizuho_Data->SetMarkerStyle(2);
	SPICE_Hole->SetMarkerStyle(32);
	
	Moores_Bay_Hole_1->SetMarkerSize(globalMarkerSize);
	Moores_Bay_Hole_2->SetMarkerSize(globalMarkerSize);
	SPICE_Hole->SetMarkerSize(globalMarkerSize);
	RICE_Data->SetMarkerSize(globalMarkerSize);
	Byrd_Data->SetMarkerSize(globalMarkerSize);
	Mizuho_Data->SetMarkerSize(globalMarkerSize);
	
	Moores_Bay_Hole_1->SetMarkerColor(kBlack);
	Moores_Bay_Hole_2->SetMarkerColor(kBlack);
	RICE_Data->SetMarkerColor(kBlack);
	SPICE_Hole->SetMarkerColor(kBlack);
	Byrd_Data->SetMarkerColor(kBlack);
	Mizuho_Data->SetMarkerColor(kBlack);
	
	Moores_Bay_Hole_1->GetYaxis()->SetRangeUser(MinIndex,MaxIndex);
	Moores_Bay_Hole_1->GetXaxis()->SetRangeUser(-MaxDepth,MinDepth);
	Moores_Bay_Hole_2->GetYaxis()->SetRangeUser(MinIndex,MaxIndex);
	Moores_Bay_Hole_2->GetXaxis()->SetRangeUser(-MaxDepth,MinDepth);
	SPICE_Hole->GetXaxis()->SetRangeUser(-MaxDepth,MinDepth);
	SPICE_Hole->GetYaxis()->SetRangeUser(MinIndex,MaxIndex);
	RICE_Data->GetYaxis()->SetRangeUser(MinIndex,MaxIndex);
	RICE_Data->GetXaxis()->SetRangeUser(-MaxDepth,MinDepth);
	Byrd_Data->GetYaxis()->SetRangeUser(-MaxDepth,MinDepth);
	Byrd_Data->GetXaxis()->SetRangeUser(MinIndex,MaxIndex);
	Mizuho_Data->GetYaxis()->SetRangeUser(-MaxDepth,MinDepth);
	Mizuho_Data->GetXaxis()->SetRangeUser(MinIndex,MaxIndex);
	
	Hole_1_Fit->SetLineColor(12);
	Hole_2_Fit->SetLineColor(12);
	SPICE_Fit->SetLineColor(kBlack);
	RICE_Data_Fit->SetLineColor(kRed);
	Byrd_Data_Fit->SetLineColor(kBlue);
	Mizuho_Data_Fit->SetLineColor(kBlue);
	
	Hole_1_Fit->SetLineWidth(globalLineWidth);
	Hole_2_Fit->SetLineWidth(globalLineWidth);
	SPICE_Fit->SetLineWidth(globalLineWidth);
	RICE_Data_Fit->SetLineWidth(globalLineWidth);
	Byrd_Data_Fit->SetLineWidth(globalLineWidth);
	Mizuho_Data_Fit->SetLineWidth(globalLineWidth);
	
	Hole_1_Fit->SetLineStyle(9);
	Hole_2_Fit->SetLineStyle(1);
	RICE_Data_Fit->SetLineStyle(9);
	SPICE_Fit->SetLineStyle(1);
	Byrd_Data_Fit->SetLineStyle(9);
	Mizuho_Data_Fit->SetLineStyle(1);
	
	Moores_Bay_Hole_1->Draw("P same");
	Moores_Bay_Hole_2->Draw("P same");
	RICE_Data->Draw("P same");
	Byrd_Data->Draw("P same");
	Mizuho_Data->Draw("P same");
	SPICE_Hole->Draw("P same");
	
	Hole_1_Fit->Draw("same");
	Hole_2_Fit->Draw("same");
	SPICE_Fit->Draw("same");
	RICE_Data_Fit->Draw("same");
	Byrd_Data_Fit->Draw("same");
	Mizuho_Data_Fit->Draw("same");
	
	TLegend *legend = new TLegend(0.15,0.12,0.72,0.525);
	legend->AddEntry(Moores_Bay_Hole_1,"MB #1 (n(z) = 1.0+0.86#rho(z)) (ARIANNA 2016)","p");
	legend->AddEntry(Hole_1_Fit,result1,"l");
	legend->AddEntry(Moores_Bay_Hole_2,"MB #2 (n(z) = 1.0+0.86#rho(z)) (ARIANNA 2016)","p");
	legend->AddEntry(Hole_2_Fit,result2,"l");
	legend->AddEntry(Byrd_Data,"Byrd: Ebinuma et al (1983)","p");
	legend->AddEntry(Byrd_Data_Fit,result6,"l");
	legend->AddEntry(Mizuho_Data,"Mizuho: Ebinuma et al (1983)","p");
	legend->AddEntry(Mizuho_Data_Fit,result7,"l");
	legend->AddEntry(RICE_Data,"SP: RICE data (2004)","p");
	legend->AddEntry(RICE_Data_Fit,result3,"l");
	legend->AddEntry(SPICE_Hole,"SP: SPICE Core (n(z) = 1.0+0.86#rho(z)) (2015)","p");
	legend->AddEntry(SPICE_Fit,result8,"l");
	legend->Draw("same");

	TGraph *Moores_Bay_Hole_1_residual = new TGraph(Moores_Bay_Hole_1->GetN());
	for(Int_t i=0;i<Moores_Bay_Hole_1->GetN();++i)
	{
		Double_t x,y;
		Moores_Bay_Hole_1->GetPoint(i,x,y);
		Moores_Bay_Hole_1_residual->SetPoint(i,x,y-Hole_1_Fit->Eval(x));
	}

	TGraph *Moores_Bay_Hole_2_residual = new TGraph(Moores_Bay_Hole_2->GetN());
	for(Int_t i=0;i<Moores_Bay_Hole_2->GetN();++i)
	{
		Double_t x,y;
		Moores_Bay_Hole_2->GetPoint(i,x,y);
		Moores_Bay_Hole_2_residual->SetPoint(i,x,y-Hole_2_Fit->Eval(x));
	}

	TGraph *SPICE_Hole_residual = new TGraph(SPICE_Hole->GetN());
	for(Int_t i=0;i<SPICE_Hole->GetN();++i)
	{
		Double_t x,y;
		SPICE_Hole->GetPoint(i,x,y);
		SPICE_Hole_residual->SetPoint(i,x,y-SPICE_Fit->Eval(x));
	}

	TGraph *RICE_Data_residual = new TGraph(RICE_Data->GetN());
	for(Int_t i=0;i<RICE_Data->GetN();++i)
	{
		Double_t x,y;
		RICE_Data->GetPoint(i,x,y);
		RICE_Data_residual->SetPoint(i,x,y-RICE_Data_Fit->Eval(x));
	}

	TGraph *Byrd_Data_residual = new TGraph(Byrd_Data->GetN());
	for(Int_t i=0;i<Byrd_Data->GetN();++i)
	{
		Double_t x,y;
		Byrd_Data->GetPoint(i,x,y);
		Byrd_Data_residual->SetPoint(i,x,y-Byrd_Data_Fit->Eval(x));
	}

	TGraph *Mizuho_Data_Fit_residual = new TGraph(Mizuho_Data->GetN());
	for(Int_t i=0;i<Mizuho_Data->GetN();++i)
	{
		Double_t x,y;
		Mizuho_Data->GetPoint(i,x,y);
		Mizuho_Data_Fit_residual->SetPoint(i,x,y-Mizuho_Data_Fit->Eval(x));
	}

	p2->cd();

	Moores_Bay_Hole_1_residual->SetMarkerStyle(24);
	Moores_Bay_Hole_2_residual->SetMarkerStyle(25);
	RICE_Data_residual->SetMarkerStyle(26);
	Byrd_Data_residual->SetMarkerStyle(5);
	Mizuho_Data_Fit_residual->SetMarkerStyle(2);
	SPICE_Hole_residual->SetMarkerStyle(32);
	
	Moores_Bay_Hole_1_residual->SetMarkerSize(globalMarkerSize);
	Moores_Bay_Hole_2_residual->SetMarkerSize(globalMarkerSize);
	SPICE_Hole_residual->SetMarkerSize(globalMarkerSize);
	RICE_Data_residual->SetMarkerSize(globalMarkerSize);
	Byrd_Data_residual->SetMarkerSize(globalMarkerSize);
	Mizuho_Data_Fit_residual->SetMarkerSize(globalMarkerSize);
	
	Moores_Bay_Hole_1_residual->SetMarkerColor(kBlack);
	Moores_Bay_Hole_2_residual->SetMarkerColor(kBlack);
	RICE_Data_residual->SetMarkerColor(kBlack);
	SPICE_Hole_residual->SetMarkerColor(kBlack);
	Byrd_Data_residual->SetMarkerColor(kBlack);
	Mizuho_Data_Fit_residual->SetMarkerColor(kBlack);

	Moores_Bay_Hole_1_residual->Draw("P same");
	Moores_Bay_Hole_2_residual->Draw("P same");
	SPICE_Hole_residual->Draw("P same");
	RICE_Data_residual->Draw("P same");
	Byrd_Data_residual->Draw("P same");
	Mizuho_Data_Fit_residual->Draw("P same");
	
	char fileOut[100];
	sprintf(fileOut,"June15_plot1.pdf");
	c->Print(fileOut);
}
