<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class Connection extends Model
{
    use HasFactory;

    public $timestamps = false;
    
    protected $fillable = [
        'value',
        'hour',
        'day',
        'interval',
    ];
}
