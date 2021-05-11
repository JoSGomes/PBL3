<?php

namespace App\Http\Livewire;

use Livewire\Component;

class FormContact extends Component
{

    public $name;
    public $number;

    public function render()
    {
        return view('livewire.form-contact');
    }

    public function create(){
        //
    }
}
