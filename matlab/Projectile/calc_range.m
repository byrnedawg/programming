function range=calc_range(v0,theta,t_flight)

  % Calculate projectile range
  %
  % Usage: range=calc_range(v0,theta,t_flight)
  %
  % where: v0 = initial velocity (magnitude)
  %        theta = launch angle (degrees)
  %        t_flight = time of flight

  range=v0*cos(pi*(theta/180))*t_flight;