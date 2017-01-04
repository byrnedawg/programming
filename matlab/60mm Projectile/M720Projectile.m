  % Projectile motion simulation

  x0=0; % m
  y0=-77.64; % m
  Charge = 0;
  mils = 1095; % Mils
  theta = mils/17.7777777778; % de
  
  if(Charge == 0)
          v0 = 65; % m/s
          g= 9.9; % m/s^2
          if(y0 >= 0)
            k = 0.018;
          else
            k = 0.015;
          end
  else
          v0 = 126; % m/s
          g= 10.2; % m/s^2
          k = 0.01
  end   
 
  t_flight=calc_t_flight(Charge, mils)
  range=calc_range(Charge, mils)
  v_impact=calc_v_impact(g,v0,theta,t_flight,y0)

  t=linspace(0,t_flight+(y0*k),300);
  xdot0=v0*cos(pi*(theta/180));
  ydot0=v0*sin(pi*(theta/180));
  
  x=(range/t_flight)*t+x0;
  y=-(g/2)*t.^2+ydot0*t+y0;
  
  
  plot(x,y)
  axis equal
  xlabel(sprintf('Distance (m) - Range = %5.3f',range));
  ylabel('Height (m)');
  title(sprintf('Projectile motion: v_{impact} = %5.3f m/s',v_impact));