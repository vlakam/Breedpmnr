<?
class parser extends CI_Controller{
    function __construct(){
        parent::__construct();
    }
    public function index()
    {
        $this->load->library('user_agent');
        $this->load->library("upload");
        if($this->agent->agent_string()!='Breed/1.0')die("Wrong Client");
        else{
            $config['upload_path'] = APPPATH.'public/i/';
            $config['allowed_types'] = 'jpg|jpeg';
            $name = substr(md5(time()), -28+rand(0,5));
            $config['file_name']=$name;
            $this->load->library('upload');
            $this->upload->initialize($config);
            if(!$this->upload->do_upload('imagedata'))
            {
                switch (ENVIRONMENT)
                {
                    case 'development':echo $this->upload->display_errors(); break;
                    case 'testing':
                    case 'production':
                        echo "Upload Failed";
                        break;
                }
            }
            else{
                    if($this->upload->is_image()) echo base_url()."i/".$this->upload->file_name;
                    else {
                        switch (ENVIRONMENT)
                        {
                            case 'development':print_r( $this->upload->data()); break;
                            case 'testing':
                            case 'production':
                                echo "Upload Failed";
                                break;
                        }
                    }
            }
        }
    }
}
?>