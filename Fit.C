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
	TGraph2D *gr2 = new TGraph2D(N);
	TGraph *gr1 = new TGraph(N);
	TGraph *gr0 = new TGraph(N);
	TF1 *f1 = new TF1("f1","atan([0]*(x-[1]))-[2]",12,90);
	f1->SetParameter(0,3.14/180.0*20.0);
	f1->SetParameter(1,12);
	f1->SetParameter(2,3.14159/2+0.05);
	TF1 *f0 = new TF1("f0","-atan([0]*(x-[1]))-3.14159/2.0",-450,0);
	f0->SetParameter(0,0.5816);
	f0->SetParameter(1,20.0);
	for(Int_t i=0;i<N;++i)
	{
		t->GetEntry(i);
		if(z==-200.0) gr1->SetPoint(i,theta,delta_theta/theta);
		if(fabs(theta-57.0)<0.1) gr0->SetPoint(i,z,delta_theta/theta);
		gr2->SetPoint(i,z,theta,delta_theta/theta);
	}
	gr0->Draw("AP");
	gr0->Fit("f0","R");
	f0->Draw("same");
	// gr1->Draw("AP");
	// gr1->Fit("f1","R");
	// f1->Draw("same");
	// gr2->Draw("P");
}