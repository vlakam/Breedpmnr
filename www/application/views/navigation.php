<div class="container">
    <nav class="navbar navbar-default">
        <a class="navbar-brand" href="#">
            <img src="<?=base_url("application/public");?>/images/top_logo.png" alt="logo"/>            
        </a>
        <ul class="nav navbar-nav pull-right">
            <li <?if($page==1) echo 'class="active"';?> > <a href="<?=base_url();?>main">Главная страница</a></li>
            <li class="disabled"><a href="#">Личный кабинет</a></li>
            <li <?if($page==3) echo 'class="active"';?> > <a href="<?=base_url();?>main/authors">Авторы</a></li>
        </ul>
    </nav>
</div> 