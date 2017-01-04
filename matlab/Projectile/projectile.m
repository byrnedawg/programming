  % Projectile motion simulation

  x0=0; % m
  y0=500; % m
  v0=100; % m/s
  theta=70; % deg
  g=9.81; % m/s^2

  t_flight=calc_t_flight(g,v0,theta,y0)
  range=calc_range(v0,theta,t_flight)
  v_impact=calc_v_impact(g,v0,theta,t_flight,y0)

  t=linspace(0,t_flight,30);
  xdot0=v0*cos(pi*(theta/180));
  ydot0=v0*sin(pi*(theta/180));
  x=xdot0*t+x0;
  y=-(g/2)*t.^2+ydot0*t+y0;
  plot(x,y)
  axis equal
  xlabel(sprintf('Distance (m) - Range = %5.3f',range));
  ylabel('Height (m)');
  title(sprintf('Projectile motion: v_{impact} = %5.3f m/s',v_impact));