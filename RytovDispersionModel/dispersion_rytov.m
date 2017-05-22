%(100) LA Superlattice dispersion

clear all

warning off
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%User defined variables

%Note, Layer A is GaAs and layer B is AlAs 
    
freq=linspace(1e9,1000e9,5000); %Phonon frequency range over which to calculate dispersion

da=22; %layer widths in monolayers
db=18; 

c11a=12.17e10; %Elastic constants
c11b=12.02e10;
c44a=6e10;
c44b=6.16e10;

la=5.6423e-10; %Lattice Parameters (5.65356e-10 at room temp)
lb=5.6528e-10; %(5.66139 at room temperature)

roha=5350.7; %5317 at room temp.
rohb=3776; %3760 at room temp.
%roha = 4*(74.9216+69.72)/(6.02214179e26*(lGaAs^3)); %Calculate density from atomic masses and lattice param
%rohb = 4*(74.9216+26.98154)/(6.02214179e26*(lAlAs^3)); %Zinc-Blende has 4 of each atom per unit cell.

LAorTA=1; %1 for LA mode, anything else for TA mode

outfname='dispersion.dat'; %Output file with easyplot compatible header

%End user variables

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

if (LAorTA == 1)
    ea=c11a;
    eb=c11b;
else
    ea=c44a;
    eb=c44b;
end

va=sqrt(ea/roha); %Calculates velocity of sound in layers from elastic constant and density
vb=sqrt(eb/rohb);
da=da*la/2;
db=db*lb/2;

%Rytov Formula
K=(roha*va)/(rohb*vb);
Q=cos(2*pi*freq*da/va).*cos(2*pi*freq*db/vb) - ((1+K*K)/(2*K))*sin(2*pi*freq*da/va).*sin(2*pi*freq*db/vb);
I=find(Q <= 1 & Q >= -1);
F=freq(I);
Q=Q(I);
Q=abs(acos(Q)/pi); %Wavevector in units of pi/d

%Plot
plot(Q,F*1e-9);
ylabel('Phonon Frequency / GHz');
xlabel('Wavenumber in units of pi/D');

%Output to Easyplot
fid=fopen(outfname,'w');
fprintf(fid,'%s\t%s\r\n','/sm','OFF');
fprintf(fid,'%s\t%s\r\n','/sc','ON');
fprintf(fid,'%s\t%s\r\n','/sd','OFF');
fprintf(fid,'%s\t%s\t%s\r\n','/fpo','pd','8.00 6.00');
fprintf(fid,'%s\t%s\r\n','/td','"xy"');
fprintf(fid,'%d\t%d\r\n',[Q;1e-9*F]);
fprintf(fid,'%s\t%s\r\n','/or',['y ' num2str(max(F)*1e-9) ' ' num2str(min(F)*1e-9)]); %Axis range
fprintf(fid,'%s\t%s\r\n','/or','x 0 1');
fprintf(fid,'%s\t%s\t%s\r\n','/et','y','"Frequency / GHz"');
fprintf(fid,'%s\t%s\t%s\r\n','/et','x','"Wavevector / (\{pi}/D)"');
%fprintf(fid,'%s\t%s\t%s\r\n','/et','g',gtitle);
fclose(fid);