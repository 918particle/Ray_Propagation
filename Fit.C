void Fit(char const *title)
{
	TNtuple *t = new TNtuple("t","","z:theta:theta_m:delta_theta");
	t->ReadFile(title);
	Float_t z;
	Float_t theta;
	Float_t delta_theta;
	t->SetBranchAddress("z",&z);
	t->SetBranchAddress("theta",&theta);
	t->SetBranchAddress("delta_theta",&delta_theta);
	Int_t N = t->GetEntries();
	//All data at once
	TH2F *gr_all = new TH2F("gr_all","log_{10}(|#Delta#theta_{i}|)",200,-1000.0,0.0,180,-90.0,90.0);
	for(Int_t i=0;i<N;++i)
	{
		t->GetEntry(i);
		float result = log10(fabs(delta_theta));
		if(result<-3.0) result=-3.0;
		gr_all->Fill(z,theta,result);
	}
	Float_t max = gr_all->GetMaximum();
	//Any bins not set in this process should be set to max error
	for(Int_t i=0;i<gr_all->GetNbinsX();++i)
	{
		for(Int_t j=0;j<gr_all->GetNbinsY();++j)
		{
			if(gr_all->GetBinContent(i,j)==0.0) gr_all->SetBinContent(i,j,max);
		}
	}

	// Plotting 2D general results
	const Int_t N_c = 9;
	Double_t N_c_f = 9.0;
	Double_t c_i = -3.0;
	Double_t c_f = 1.5;

	Double_t c[N_c];
	Double_t dc = (c_f - c_i)/N_c_f;
	for(Int_t i=0;i<N_c;++i) c[i] = c_i+i*dc;
	gr_all->SetContour(N_c,c);
	gr_all->SetMinimum(c_i);
	gr_all->SetMaximum(c_f);
	gStyle->SetOptStat(0);
	gr_all->GetXaxis()->SetTitle("Depth (m)");
	gr_all->GetYaxis()->SetTitle("#theta_{i}");
	gr_all->Draw("cont4z");
}
	//All data at once, fractional
	// TGraph2D *gr2 = new TGraph2D(N);
	//Just one variable, with the other held constant
	// TGraph *gr0 = new TGraph(N);
	// TGraph *gr1 = new TGraph(N);
	//Fitting functions for the single-variable cases
	//Varying z while holding theta constant
	// TF1 *f0 = new TF1("f0","-[0]*exp(x/[1])",-800,-35);
	// f0->SetParameter(0,0.75);
	// f0->SetParameter(1,50.0);
	//Varuing theta whjle holding z constant
	// TF1 *f1 = new TF1("f1","atan([0]*(x-[1]))-[2]",5,90);
	// f1->SetParameter(0,3.14/180.0*20.0);
	// f1->SetParameter(1,12);
	// f1->SetParameter(2,3.14159/2+0.05);
	//Fitting function for the 2D general case
	// TF2 *f2 = new TF2("f2","(-[0]*exp(x/[1])) * (atan([2]*(y-[3]))-[4])",-450,-35,12,90);
	// f2->SetParameter(0,0.75);
	// f2->SetParameter(1,50.0);
	// f2->SetParameter(2,3.14/180.0*20.0);
	// f2->SetParameter(3,12);
	// f2->SetParameter(4,3.14159/2+0.05);

	// if(fabs(theta-80.0)<0.5) gr0->SetPoint(i,z,delta_theta/theta);
	// if(z==-400.0) gr1->SetPoint(i,theta,delta_theta/theta);
	// gr2->SetPoint(i,z,theta,delta_theta/theta);

	// gr0->Draw("AP");
	// gr0->Fit("f0","R");
	// f0->Draw("same");
	// gr1->Draw("AP");
	// gr1->Fit("f1","R");
	// f1->Draw("same");

	// gr2->GetXaxis()->SetRangeUser(-1000,0);
	// gr2->GetYaxis()->SetRangeUser(0,90);
	// gr2->GetZaxis()->SetRangeUser(-10,1.0);
	// gr2->SetMaximum(1.0);
	// gr2->SetMinimum(-10.0);
	// gr2->Draw("colz");
	// gr2->Fit("f2","R");
	// f2->SetMaximum(0.1);
	// f2->SetMinimum(-1.1);
	// f2->GetXaxis()->SetRangeUser(-400,-50);
	// f2->GetYaxis()->SetRangeUser(10,80);
	// f2->GetZaxis()->SetRangeUser(-1.1,0.1);
	// f2->Draw("surf same");