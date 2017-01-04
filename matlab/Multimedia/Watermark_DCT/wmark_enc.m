function varargout = wmark_enc(varargin)
% WMARK_ENC MATLAB code for wmark_enc.fig
%      WMARK_ENC, by itself, creates a new WMARK_ENC or raises the existing
%      singleton*.
%
%      H = WMARK_ENC returns the handle to a new WMARK_ENC or the handle to
%      the existing singleton*.
%
%      WMARK_ENC('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in WMARK_ENC.M with the given input arguments.
%
%      WMARK_ENC('Property','Value',...) creates a new WMARK_ENC or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before wmark_enc_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to wmark_enc_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help wmark_enc

% Last Modified by GUIDE v2.5 01-Apr-2014 14:30:29

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @wmark_enc_OpeningFcn, ...
                   'gui_OutputFcn',  @wmark_enc_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before wmark_enc is made visible.
function wmark_enc_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to wmark_enc (see VARARGIN)

% Choose default command line output for wmark_enc
handles.output = hObject;
axes(handles.iim); axis off
axes(handles.oim); axis off
axes(handles.omsg); axis off
axes(handles.demsg); axis off
set(handles.emmsg,'Enable','off')
set(handles.extmsg,'Enable','off')
set(handles.msg,'Enable','off')

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes wmark_enc wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = wmark_enc_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in exit.
function exit_Callback(hObject, eventdata, handles)
% hObject    handle to exit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
close wmark_enc

% --- Executes on button press in extmsg.
function extmsg_Callback(hObject, eventdata, handles)
% hObject    handle to extmsg (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
embimg=handles.embimg;
wm=exwmark(embimg);
axes(handles.demsg); imshow(wm); title('Extracted MSG')
handles.wm=wm;
guidata(hObject,handles)


% --- Executes on button press in emmsg.
function emmsg_Callback(hObject, eventdata, handles)
% hObject    handle to emmsg (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
img=handles.img; msg=handles.msg; 
h=warndlg('Wait....','Processing');
[embimg,ps]=wtmark(img,msg);
handles.embimg=embimg;
axes(handles.oim); imshow(embimg); title('Embedded Image')
set(handles.impsnr,'String',ps)
set(handles.extmsg,'Enable','On')
close(h)
guidata(hObject,handles)

% --- Executes on button press in inp.
function inp_Callback(hObject, eventdata, handles)
% hObject    handle to inp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
[fname path]=uigetfile({'*.jpg';'*.bmp';,'*.jpeg';'*.tiff';'*.png'},'Browse Image');
if fname~=0
    img=imread([path,fname]);
    if length(size(img))>2
        img=rgb2gray(img);
    end
    axes(handles.iim); imshow(img);
    title('Orignal Image')
    handles.img=img;
    set(handles.msg,'Enable','on')
else
    warndlg('Please Select Image File');
end
guidata(hObject,handles);

% --- Executes on button press in msg.
function msg_Callback(hObject, eventdata, handles)
% hObject    handle to msg (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
[fname path]=uigetfile({'*.jpg';'*.bmp';,'*.jpeg';'*.tiff';'*.png'},'Browse Image');
if fname~=0
    msg=imread([path,fname]);
    if length(size(msg))>2
        msg=rgb2gray(msg);
    end
    axes(handles.omsg); imshow(msg);
    title('MSG')
    handles.msg=msg;
    set(handles.emmsg,'Enable','on')
else
    warndlg('Please Select Image File');
end
guidata(hObject,handles);
