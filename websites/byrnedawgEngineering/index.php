<?php
	if (isset($_POST["submit"])) {
		$name = $_POST['name'];
		$email = $_POST['email'];
		$message = $_POST['message'];
		$human = intval($_POST['human']);
		$from = 'Website Contact Form'; 
		$to = 'byrnedawg95@gmail.com'; 
		$subject = 'Message from Byrnedawg Website';
		
		$body ="From: $name\n E-Mail: $email\n Message:\n $message";
		// Check if name has been entered
		if (!$_POST['name']) {
			$errName = 'Please enter your name';
		}
		
		// Check if email has been entered and is valid
		if (!$_POST['email'] || !filter_var($_POST['email'], FILTER_VALIDATE_EMAIL)) {
			$errEmail = 'Please enter a valid email address';
		}
		
		//Check if message has been entered
		if (!$_POST['message']) {
			$errMessage = 'Please enter your message';
		}
		//Check if simple anti-bot test is correct
		if ($human !== 5) {
			$errHuman = 'Your anti-spam is incorrect';
		}
// If there are no errors, send the email
if (!$errName && !$errEmail && !$errMessage && !$errHuman) {
	if (mail ($to, $subject, $body, $from)) {
		$result='<div class="alert alert-success">Thank You! I will be in touch</div>';
	} else {
		$result='<div class="alert alert-danger">Sorry there was an error sending your message. Please try again later.</div>';
	}
}
	}
?>
<!DOCTYPE html>
<html>
<head>
  <title>Byrnedawg Engineering</title>
  <link rel="icon" href="imgs/ByrnedawgLogoGrayCompact.png">
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
  <style>
   body {
      /*margin: auto;*/
      position: relative; 
      /*margin-top: 50px;*/
  }
  .affix {
      top:0;
      width: 100%;
      z-index: 9999 !important;
  }
  .navbar {
      margin-bottom: 0px;
  }

  .affix ~ .container-fluid {
     position: relative;
     top: 50px;
  }
  img {
    cursor: zoom-in;
}

h1{
  font-family: "Arial Black", Gadget, sans-serif
  
}

.no-padding {
  padding-left: 0;
  padding-right: 0;
}


.row{
    padding-top: 10px;
    padding-bottom: 10px;
    margin-left:  10px;
    margin-right: 10px;
}

.fixed-height {
    max-height: 400px;
}

.navbar-brand>img {
   max-height: 100%;
   height: 100%;
   width: auto;
   margin: 0 auto;
   pointer-events: none;


  
   -o-object-fit: contain;
   object-fit: contain; 

}

/*
.jumbotron {
  position: absolute;
  top: 50%;
  left:50%;
  transform: translate(-50%,-50%);
  border: 1px dashed green;
  
}
*/
  #logo {padding-top:10px; padding-bottom:10px; cursor:default; pointer-events: none;}
  #nav1 {padding-top:1px; padding-bottom:1px;}
  #section1 {padding-top:20px;padding-bottom:20px; color: white; background: rgb(48,48,48);}
  #section2 {padding-top:10px;padding-bottom:20px; color: white; background: url(imgs/gradientcheckerdouble.png);}
  #section3 {padding-top:10px;padding-bottom:20px; color: white; background-color: rgb(48,48,48);}
  #section4 {padding-top:10px;padding-bottom:20px; color: white; background: url(imgs/gradientcheckerdouble.png);}
  #section5 {padding-top:10px;padding-bottom:20px; color: white; background: rgb(48,48,48);}
  #footer   {padding-top:10px;padding-bottom:20px; color: black; background-color: LightGray;}
  
  
  .carousel-inner > .item > img,
  .carousel-inner > .item > a > img {
     /* width: 70%;*/
      height:350px;
      margin: auto;
  }
  
  .navbar .navbar-nav {
  display: inline-block;
  float: none;
  vertical-align: top;
}

.navbar .navbar-collapse {
  text-align: center;
}
  
  @media (min-width: 768px) {
    .navbar .navbar-nav {
        display: inline-block;
        float: none;
        vertical-align: top;
    }

    .navbar .navbar-collapse {
        text-align: center;
    }
    
    .fixed-height {
    max-height: 350px;
}
}

@media (min-width: 576px) {
   .fixed-height {
    max-height: 325px;
  }
}

@media (min-width: 992px) {
   .fixed-height {
    max-height: 375px;
  }

}

@media (min-width: 1200px) {
   .fixed-height {
    max-height: 400px;
  }

}

  </style>
</head>
<body data-spy="scroll" data-target=".navbar" data-offset="50">

<div class="container-fluid no-padding" style="color: black; background-color: Black; padding-top: 5px; padding-bottom: 5px;" >
  <div class="container-fluid no-padding" style="background-image: url(imgs/byrnedawgEngineeringLogoRed.png); background-repeat: repeat-x; min-height: 100px;">
      <div class="container-fluid no-padding">
          <img id = "logo" class="img-responsive center-block no-padding" style = "no-repeat;  min-height: 100px;" src="imgs/byrnedawgEngineeringLogo2.jpg" alt="Logo"> 
      </div>
  </div>
</div>


<nav id = "nav1" class="navbar navbar-inverse" data-spy="affix" data-offset-top="110"> <!-- navbar-fixed-top-->
  <div class="container-fluid">
    <div class="navbar-header">
      <a class="navbar-brand" href="#logo">
         <img src="imgs/ByrnedawgLogoGrayCompact.bmp" class="center-block " style="display:inline-block;" alt=""> <span>Byrnedawg Engineering</span>
      </a>
      <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#myNavbar">
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
      </button>
    </div>
    <div>
      <div class="collapse navbar-collapse " id="myNavbar">
        <ul class="nav navbar-nav">
          <li><a href="#section1">About</a></li>
          <li><a href="#section2">Services</a></li>
          <li><a href="#section3">Projects</a></li>
          <li><a href="#contact">Contact</a></li>
          <li><a href="#section5">Links</a></li>
        </ul>
      </div>
    </div>
  </div>
</nav>    

<div id="section1" class="container-fluid">
  <div style = "padding-top:10%;padding-bottom:10%;background: url(imgs/blueCircuitBackground7.jpg) center no-repeat; Background-size:225% auto;">
  <!--  <div style = "padding-top:60px;padding-bottom:60px;background: url(http://media.istockphoto.com/photos/futuristic-circuit-board-blue-with-electrons-picture-id483147081?k=6&m=483147081&s=612x612&w=0&h=qUkUJKX1QvMbWv1Y2xQzgC6we1YiSLomxR1_h-25Fdk=)">--> 
    <h1 class = "text-center" style = "font-size: 60px; color: white"> Welcome</h1>
    <div class = "container center-block">
      <div class = "row">
        <div class = "col-sm-2 col-md-2 col-lg-2">
        </div>
        <div class = "col-sm-8 col-md-8 col-lg-8">
          <div class = "center-block well" style = "background-color: rgba(48, 48, 48, .5);">
            <p class = "text-center" style = "font-size: 18px; color: white"> <strong>Need to turn your great idea into a new Product?<br>That's where Byrnedawg Engineering excels!
</strong>  </p>
          </div>
        </div>
      <div class = "col-sm-2 col-md-2 col-lg-2">
      </div>
      </div>
    </div>
    <!--<p class = "text-center" style = "padding-top:60px; font-size: 18px; color: white"> <strong>Everyone has ideas but not everyone is an Engineer...<br>Thats where Byrnedawg Engineering comes in.</strong>  </p> -->
    <!--<p class = "text-left" style = "color: white">Rather than wait for someone else or some company to develop and make money off your idea, lets team up and convert your idea into a product or design solution today. </p> -->
  </div>
  <div class="container" style = "background: rgb(48,48,48);">
    <h1 class = "text-center">ABOUT</h1>
    <div>
          <div class = "row">
            <div class = "col-sm-2 col-md-2 col-lg-2">
            </div>
            <div class = "well col-sm-8 col-md-8 col-lg-8 text-left" style = "background: rgba(128, 128, 128, .5);">
                <p> Gregory Byrne, M.E., B.S.</p>
                <ul>
                    <li>Master of Engineering in Computer Engineering Stevens Institute of Technology</li>
                    <li>Bachelor of Science in Electrical and Computer Engineering Rowan University</li>
                    <li>Currently employed by the Department of Defense</li>
                    <li>Engineering experience throughout the full life cycle of a product</li>
                </ul>
                <p>If you need an experienced, innovative, out-of-the-box thinker, well-versed in electrical and computer engineering to bring your product to fruition, then you need Byrnedawg Engineering!</p>
            </div>
            <div class = "col-sm-2 col-md-2 col-lg-2">
            </div>
      </div>
  </div>
</div>
<div id="section2" class="container-fluid">
  <h1 class = "text-center">SERVICES</h1>
  <div class = "row">
    <div class = "col-lg-2">
    </div>
    <div class = "col-sm-6 col-md-6 col-lg-4">
        <div class = "well" style = "background: rgba(128, 128, 128, .5);">
          <p><strong>How We Convert Your Idea into a New Product</strong></p>
          <ul>
            <li>Consult with a Electrical/Computer Engineer to determine design feasibility</li>
            <li>Prototype development for evaluation and proof of concept</li>
            <li>Refine prototype if necessary to meet market and industry standards</li>
            <li>Technical Data Package of Design Solution to move forward with patent application or direct production</li>
          </ul>
        </div>
    </div>
    <div class = "col-sm-6 col-md-6 col-lg-4">
      <img class=" img-responsive img-rounded center-block fixed-height" src="imgs/RSAV2.jpg" alt="RSAV">
    </div>
  </div>
  <div class = "col-lg-2">
  </div>
  <div class = "row"> 
    <div class = "col-lg-2">
    </div>
    <div class = "col-sm-6 col-md-6 col-lg-4 col-sm-push-6 col-md-push-6 col-lg-push-4">
        <div class = "well center-block" style = "background: rgba(128, 128, 128, .5);">
          <p><strong>Technical Skills</strong></p>
          <ul>
            <li>Electrical Circuit Design and Analysis</li>
            <li>Design and Assembly of Printed Circuit Boards</li>
            <li>Engineering Drawings including 3D Modeling</li>
          </ul>
        </div>
    </div>
    <div class = "col-sm-6 col-md-6 col-lg-4 col-sm-pull-6 col-md-pull-6 col-lg-pull-4">
      <img class=" img-responsive img-rounded center-block fixed-height" src="imgs/solder1.JPG" alt="Soldering" >
    </div>
    <div class = "col-lg-2">
    </div>
  </div>
  <div class = "row">    
    <div class = "col-lg-2">
    </div>
    <div class = "col-sm-6 col-md-6 col-lg-4">
        <div class = "well center-block" style = "background: rgba(128, 128, 128, .5);">
          <p><strong>Website Development</strong></p>
          <ul>
             <li>Personalized Website Development</li>
             <ul>
                <li>For your business</li>
                <li>For your product</li>
             </ul>
          </ul>
        </div>
    </div>
    <div class = "col-sm-6 col-md-6 col-lg-4">
      <img class=" img-responsive img-rounded center-block fixed-height" src="imgs/ledDetector2.JPG" alt="LED Detector PCB">
    </div>
    <div class = "col-lg-2">
    </div>
  </div>
 <div class = "row"> 
    <div class = "col-lg-2">
    </div>
    <div class = "col-sm-6 col-md-6 col-lg-4 col-sm-push-6 col-md-push-6 col-lg-push-4">
        <div class = "well center-block" style = "background: rgba(128, 128, 128, .5);">
          <p><strong>Programming Languages:</strong></p>
          <ul>
            <li>MATLAB</li>
            <li>C</li>
            <li>C++</li>
            <li>Java</li>
            <li>Python</li>
            <li>Verilog</li>
            <li>Javascript</li>
            <li>SQL</li>
          </ul>
        </div>
    </div>
     <div class = "col-sm-6 col-md-6 col-lg-4 col-sm-pull-6 col-md-pull-6 col-lg-pull-4">
      <img class=" img-responsive img-rounded center-block fixed-height" src="imgs/linkedList2CPP2.JPG" alt="CPP Program" >
    </div>
    <div class = "col-lg-2">
    </div>
  </div>
 <div class = "row">   
   <div class = "col-lg-2">
   </div>
   <div class = "col-sm-6 col-md-6 col-lg-4">
       <div class = "well center-block" style = "background: rgba(128, 128, 128, .5);">
         <p><strong>Web Technologies:</strong></p>
          <ul>
            <li>HTML5</li>
            <li>CSS</li>
            <li>Bootstrap</li>
            <li>Angular</li>
            <li>XML</li>
            <li>jQuery</li>
          </ul>
        </div>
    </div>
    <div class = "col-sm-6 col-md-6 col-lg-4">
      <img class=" img-responsive img-rounded center-block fixed-height" src="imgs/raspberrypi1.JPG" alt="Raspberry Pi" >
    </div>
    <div class = "col-lg-2">
    </div>
  </div>
 <div class = "row">
   <div class = "col-lg-2">
    </div>
    <div class = "col-sm-6 col-md-6 col-lg-4 col-sm-push-6 col-md-push-6 col-lg-push-4">
        <div class = "well center-block" style = "background: rgba(128, 128, 128, .5);">
          <p><strong>Embedded Architectures:</strong></p>
          <ul>
            <li>Atmel AVR</li>
            <li>Arduino</li>
            <li>Raspberry Pi </li>
            <li>Beaglebone</li>
            <li>Microchip Pic</li>
            <li>TI MSP430</li>
            <li>Intel 8051</li>
            <li>Altera FPGAs</li>
          </ul>
        </div>
    </div>
    <div class = "col-sm-6 col-md-6 col-lg-4 col-sm-pull-6 col-md-pull-6 col-lg-pull-4">
      <img class=" img-responsive img-rounded center-block fixed-height" src="imgs/atmelstk6001.JPG" alt="AVR STK600" >
    </div>
    <div class = "col-lg-2">
    </div>
  </div>
</div>
<div id="section3" class="container-fluid">
<h1 class = "text-center">PROJECTS</h1>
  <div class = "row">
   <div class = "col-sm-2 col-md-2 col-lg-2">
   </div>
    <div class = "col-sm-8 col-md-8 col-lg-8">
      <div class = "well center-block" style = "background: rgba(128, 128, 128, .5);">
        <p class = "text-center">Recent Projects</p>
        <div class = "row">
          <div class = "col-md-1 col-lg-2">
          </div>
         <div class = "col-sm-6 col-md-5 col-lg-4">
          <ul>
            <li>RSAV Remote Situational Awarness Vehicle</li>
            <li>Auto On RGB Night Light</li>
            <li>PS2 Remote Control Boat</li>
            <li>Digital Inclinometer</li>
          </ul> 
         </div>
         <div class = "col-sm-6 col-md-5 col-lg-4">
           <ul>
            <li>Timer for Water Pump</li>
            <li>Custom Stepper Motor Driver</li>
            <li>Open GL 3D Model STL Generator</li>
           </ul>
         </div>
         <div class = "col-md-1 col-lg-2">
         </div>
        </div>
       </div>
      </div>
    <div class = "col-sm-2 col-md-2 col-lg-2">
   </div>
  </div>
  <div id="myCarousel" class="carousel slide" data-ride="carousel">
    <!-- Indicators -->
    <ol class="carousel-indicators">
      <li data-target="#myCarousel" data-slide-to="0" class="active"></li>
      <li data-target="#myCarousel" data-slide-to="1"></li>
      <li data-target="#myCarousel" data-slide-to="2"></li>
      <li data-target="#myCarousel" data-slide-to="3"></li>
      <li data-target="#myCarousel" data-slide-to="4"></li>
      <li data-target="#myCarousel" data-slide-to="5"></li>
      <li data-target="#myCarousel" data-slide-to="6"></li>
      <li data-target="#myCarousel" data-slide-to="7"></li>
      <li data-target="#myCarousel" data-slide-to="8"></li>
      <li data-target="#myCarousel" data-slide-to="9"></li>
      <li data-target="#myCarousel" data-slide-to="10"></li>
      <li data-target="#myCarousel" data-slide-to="11"></li>
      <li data-target="#myCarousel" data-slide-to="12"></li>
      <li data-target="#myCarousel" data-slide-to="13"></li>
      <li data-target="#myCarousel" data-slide-to="14"></li>
      <li data-target="#myCarousel" data-slide-to="15"></li>
      <li data-target="#myCarousel" data-slide-to="16"></li>
      <li data-target="#myCarousel" data-slide-to="17"></li>
    </ol>

    <!-- Wrapper for slides -->
    <div class="carousel-inner" role="listbox">
      <div class="item active fixed-height">
        <img src="imgs/RSAV2.JPG" alt="RSAV">
      </div>

      <div class="item fixed-height">
        <img src="imgs/RSAV2.jpg" alt="RSAV2">
      </div>

      <div class="item fixed-height">
        <img src="imgs/scope.JPG" alt="Scope">
      </div>

      <div class="item fixed-height">
        <img src="imgs/servoCounter2.jpg" alt="ServoCounter">
      </div>
      
      <div class="item fixed-height">
        <img src="imgs/d-latchLedit.jpg" alt="D-Latch L edit">
      </div>
      
      <div class="item fixed-height">
        <img src="imgs/waterPump1.JPG" alt="Water Pump Timer">
      </div>
      
      <div class="item fixed-height">
        <img src="imgs/waveletimgcomp.jpg" alt="Wavelet Compression">
      </div>
      
      <div class="item fixed-height">
        <img src="imgs/Altera1.jpg" alt="Altera Board">
      </div>
      
      <div class="item fixed-height">
        <img src="imgs/boat1.JPG" alt="Remote Boat 1">
      </div>
      
       <div class="item fixed-height">
        <img src="imgs/boat3.JPG" alt="Remote Boat Engine">
      </div>
      
       <div class="item fixed-height">
        <img src="imgs/breakoutPCB1.JPG" alt="Breakout PCB">
      </div>
       <div class="item fixed-height">
        <img src="imgs/breadBoard1.JPG" alt="BreadBoardin">
      </div>
      
      <div class="item fixed-height">
        <img src="imgs/ps2Controller1.JPG" alt="PS2 Controller">
      </div>
      
      <div class="item fixed-height">
        <img src="imgs/dual-H-Bridge.JPG" alt="Dual H-Bridge">
      </div>
      
      <div class="item fixed-height">
        <img src="imgs/beaglebone1.JPG" alt="BeagleboneBlack">
      </div>
      
       <div class="item fixed-height">
        <img src="imgs/pcb2.JPG" alt="PCB2">
      </div>
      
      <div class="item fixed-height">
        <img src="imgs/protoBoard1.JPG" alt="ProtoBoard">
      </div>
      
      <div class="item fixed-height">
        <img src="imgs/opengl.jpg" alt="OpenGL">
      </div>
      
    </div>     

    <!-- Left and right controls -->
    <a class="left carousel-control" href="#myCarousel" role="button" data-slide="prev">
      <span class="glyphicon glyphicon-chevron-left" aria-hidden="true"></span>
      <span class="sr-only">Previous</span>
    </a>
    <a class="right carousel-control" href="#myCarousel" role="button" data-slide="next">
      <span class="glyphicon glyphicon-chevron-right" aria-hidden="true"></span>
      <span class="sr-only">Next</span>
    </a>
  </div>
</div>
  <!-- Container (Contact Section) -->
<div id="contact" class="container-fluid" style = "color: white; padding-top:10px; padding-bottom:10px; background-color: rgb(144,144,144);">
    <h1 class = "text-center">CONTACT</h1>
    <div class="row">
  	    <div class="col-sm-5 col-md-4">
  	         <div class = "well text-center " style = "color: black;">
                <p><span class="glyphicon glyphicon-map-marker"></span> New Jersey, US</p>
                <p><span class="glyphicon glyphicon-phone"></span> 732-742-5508</p>
                <p><span class="glyphicon glyphicon-envelope"></span> byrnedawg95@gmail.com</p>
            </div>
        </div>
  		<div class="col-sm-7">
			<form class="form-horizontal" role="form" method="post" action="index.php">
			    <div class="form-group">
				    <label for="name" class="col-sm-2 control-label">Name</label>
					<div class="col-sm-10">
						<input type="text" class="form-control" id="name" name="name" placeholder="First & Last Name" value="<?php echo htmlspecialchars($_POST['name']); ?>">
						<?php echo "<p class='text-danger'>$errName</p>";?>
					</div>
				</div>
				<div class="form-group">
					<label for="email" class="col-sm-2 control-label">Email</label>
					<div class="col-sm-10">
						<input type="email" class="form-control" id="email" name="email" placeholder="example@domain.com" value="<?php echo htmlspecialchars($_POST['email']); ?>">
						<?php echo "<p class='text-danger'>$errEmail</p>";?>
					</div>
				</div>
				<div class="form-group">
					<label for="message" class="col-sm-2 control-label">Message</label>
					<div class="col-sm-10">
						<textarea class="form-control" rows="4" name="message"><?php echo htmlspecialchars($_POST['message']);?></textarea>
						<?php echo "<p class='text-danger'>$errMessage</p>";?>
					</div>
				</div>
				<div class="form-group">
					<label for="human" class="col-sm-2 control-label">2 + 3 = ?</label>
					<div class="col-sm-10">
						<input type="text" class="form-control" id="human" name="human" placeholder="Your Answer">
						<?php echo "<p class='text-danger'>$errHuman</p>";?>
				    </div>
				</div>
				<div class="form-group">
					<div class="col-sm-10 col-sm-offset-2">
						<input id="submit" name="submit" type="submit" value="Send" class="btn btn-primary">
					</div>
				</div>
				<div class="form-group">
					<div class="col-sm-10 col-sm-offset-2">
						<?php echo $result; ?>	
					</div>
				</div>
			</form> 
		</div>
		<div class="col-md-1">
		</div>    
	</div>
</div>

<div id="section4" class="container-fluid">
  <div class = "row">
    <div class = "col-sm-12">
      <img class=" img-responsive center-block" src="imgs/card.jpeg" alt="Byrnedawg Card">
    </div>
  </div>
</div>


<div id="section5" class="container-fluid">
  <h1 class = "text-center">LINKS</h1>
  <div class = "text-center" style = "padding-top: 60px; padding-bottom: 60px; background: url(http://www.publicdomainpictures.net/pictures/200000/velka/circuit-board-1477415362NkY.jpg)">
    <div class = "row">
      <div class = "col-sm-2 col-md-2 col-lg-2">
      </div>
      <div class = "col-sm-8 col-md-8 col-lg-8">
        <div class = "well center-block" style = "background: rgba(48, 48, 48, .5);">
          <p style = "font-size: 25px">Websites</p>
          <ul class = "pull-left; text-left" style = "font-size: 20px">
            <li><a style = "color: white;" href="penumbraMoon.html">My Proposal Webpage</a></li>
            <li><a style = "color: white;" href="www.facebook.com/ByrnedawgEngineering">Byrnedawg Engineering Facebook Page</a></li>
          </ul>
        </div>
      </div>
      <div class = "col-sm-2 col-md-2 col-lg-2">
      </div>
    </div>
  </div>
</div>



<footer id="footer" class="container-fluid text-center">
  <a href="#logo" title="To Top">
    <span class="glyphicon glyphicon-chevron-up"></span>
  </a>
  <p>© 2017  BYRNEDAWG ENGINEERING</p>
</footer>

<!-- All this is for image modal on click -->
<div class="modal fade" id="enlargeImageModal" tabindex="-1" role="dialog" aria-labelledby="enlargeImageModal" aria-hidden="true">
  <div class="modal-dialog modal-lg" role="document">
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">×</span></button>
      </div>
      <div class="modal-body">
        <img src="" class="enlargeImageModalSource" style="width: 100%;">
      </div>
    </div>
  </div>
</div>

<!-- All this is for image modal on click -->
<script>
  $(function() {
    	$('img').on('click', function() {
			$('.enlargeImageModalSource').attr('src', $(this).attr('src'));
			$('#enlargeImageModal').modal('show');
		});
});

$(document).ready(function(){
  // Add smooth scrolling to all links in navbar + footer link
  $(".navbar a, footer a[href='#logo']").on('click', function(event) {
    // Make sure this.hash has a value before overriding default behavior
    if (this.hash !== "") {
      // Prevent default anchor click behavior
      event.preventDefault();

      // Store hash
      var hash = this.hash;

      // Using jQuery's animate() method to add smooth page scroll
      // The optional number (900) specifies the number of milliseconds it takes to scroll to the specified area
      $('html, body').animate({
        //scrollTop: $(hash).offset().top
        scrollTop: $(hash).offset().top -50
      }, 900, function(){
   
        // Add hash (#) to URL when done scrolling (default click behavior)
        window.location.hash = hash;
      });
    } // End if
  });
});  

</script>
</body>

</html>
