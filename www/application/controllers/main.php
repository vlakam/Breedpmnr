<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');
class main extends CI_Controller {
    function __construct(){
        parent::__construct();
    }
    public function index()
    {

        $data=array(
            'navbar'=>$this->load->view('navigation',array('page'=>1),true),
            'header'=>$this->load->view('header',array('title'=>'Breedpmnr'),true),
            'content'=>$this->load->view('index',array('smallbanners'=>''),true),
            'footer'=>$this->load->view('footer',array('footerbanners'=>''),true)
        );
        $this->load->view('layout\main',$data);
    }
    public function authors()
    {
        $data=array(
            'navbar'=>$this->load->view('navigation',array('page'=>3),true),
            'header'=>$this->load->view('header',array('title'=>'Breedpmnr'),true),
            'content'=>$this->load->view('authors',array('smallbanners'=>''),true),
            'footer'=>$this->load->view('footer',array('footerbanners'=>''),true)
        );
        $this->load->view('layout\main',$data);
    }

}

?>